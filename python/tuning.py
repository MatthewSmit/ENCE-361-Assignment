"""
Python module to find optimal parameters for helicopter rig.

Author: Daniel van Wichen
Date: 2017-05-27
"""
from builtins import len

import numpy
from numpy.fft import rfft, rfftfreq
from scipy import signal
import os
import re

# Path location of the data files
DATA_PATH = 'data'

# Rate in Hz of the serial output
SAMPLING_RATE = 20


def get_files(path):
    """

    :param path: the directory containing the data files
    :return: all of the txt files within the data directory
    """
    with os.scandir(path) as it:
        return [entry.path for entry in it if entry.name.endswith('.txt') and entry.is_file()]


def get_heli(filename):
    """

    :param filename: the file to process
    :return: the heli number info string
    """
    with open(filename) as infile:
        lines = infile.readlines()
        for line in lines:
            if 'heli' in line.lower():
                return line.strip()


def process_sessions(filename, n_last):
    """
    File has the following format.

    start
    data_1, time_1
    data_1, time_1
    ...
    data_1, time_1
    end [gain]

    :param filename: the file to process
    :param n_last: only process n_last sessions from this file
    :return: a dictionary mapping session id to a tuple of the form (gain, period)
    """
    with open(filename) as infile:
        text = infile.read()

    # Extract the session data from a block of text.
    pattern = re.compile('^start(.+?)^end(?: )+\[([0-9 ]+)\]$', re.MULTILINE | re.DOTALL)
    sessions = pattern.findall(text)

    if len(sessions) < n_last:
        n_last = len(sessions)

    sid_dict = {}
    for sid in reversed(range(len(sessions) - n_last, len(sessions))):
        (session, gain) = sessions[sid]

        # If session was started after a hard reset only look at the data after the last start
        (_, _, session) = session.rpartition('start')

        gain = float(gain) / 1000.0
        session_data = filter(None, map(str.strip, session.split('\n')))
        data = []
        for entry in session_data:
            dp = int(entry.split(',')[0])
            data.append(dp)

        period = find_osc_period(numpy.array(data), SAMPLING_RATE)
        sid_dict[sid + 1] = (gain, period)
    return sid_dict


def find_osc_period(data, sampling_rate):
    """

    :param data: the data (either height or yaw readings) that has been output to serial
    :param sampling_rate: rate of serial output
    :return: the period (s) of induced oscillation
    """
    data = signal.detrend(data, type='constant')
    sampling_period = 1 / sampling_rate

    yf = rfft(data)
    xf = rfftfreq(data.size, sampling_period)

    idx = numpy.argmax(abs(yf))

    return xf[idx]


def main():
    for infile in get_files(DATA_PATH):
        print('-' * 30)
        print('{} - {}'.format(get_heli(infile), infile))

        sessions = process_sessions(infile, 2)
        print('{} sessions in total\n'.format(max(sessions) if sessions else 0))
        for sid in reversed(sorted(sessions)):
            (gain, period) = sessions[sid]
            print('Session {}:'.format(sid))
            print('Ultimate gain: {}'.format(gain))
            print('Oscillation period (s): {:.3f}\n'.format(period))
        print()

if __name__ == '__main__':
    main()
