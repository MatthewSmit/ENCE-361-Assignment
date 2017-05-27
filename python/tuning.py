"""
Python module to find optimal parameters for helicopter rig.

Author: Daniel van Wichen
Date: 2017-05-27
"""

import numpy
from numpy.fft import rfft, rfftfreq
from scipy import signal
import os
import re


def get_files(path):
    """

    :param path:
    :return:
    """
    with os.scandir(path) as it:
        return [entry.path for entry in it if entry.name.endswith('.txt') and entry.is_file()]


def get_heli(filename):
    """

    :param filename:
    :return:
    """
    with open(filename) as infile:
        lines = infile.readlines()
        for line in lines:
            if 'heli' in line.lower():
                return line.strip()


def process_sessions(filename, nlast):
    """

    :param filename:
    :param nlast
    :return:
    """
    with open(filename) as infile:
        match_exp = re.compile('^start(.+?)end.+?\[(.+?)\]$', re.MULTILINE | re.DOTALL)
        sessions = re.findall(match_exp, infile.read())
        assert len(sessions) >= nlast
        sid_dict = {}
        for sid in range(1, nlast+1):
            (session, gain) = sessions[-sid]
            gain = float(gain) / 1000.0
            data = filter(None, map(str.strip, session.split('\n')))
            frequency_data = []
            times = []
            for entry in data:
                e = entry.split(',')
                frequency_data.append(int(e[0]))
                times.append(int(e[1]))

            peak = find_osc_period(numpy.array(frequency_data), 10)
            sid_dict[sid] = (gain, peak)
        return len(sessions), sid_dict


def find_osc_period(frequency_data, sampling_rate):
    """

    :param frequency_data:
    :param sampling_rate:
    :return:
    """
    frequency_data = signal.detrend(frequency_data, type='constant')
    num_samples = frequency_data.size
    sampling_period = 1 / sampling_rate

    yf = rfft(frequency_data, n=frequency_data.size)
    xf = rfftfreq(frequency_data.size, sampling_period)[:num_samples]

    (idx, _) = max(enumerate(abs(yf)), key=lambda x: x[1])

    return xf[idx]


def main():
    for infile in get_files('data'):
        print('-' * 20)
        print('{} - {}'.format(get_heli(infile), infile))

        (n, sessions) = process_sessions(infile, 1)
        print('{} sessions in total\n'.format(n))
        for sid in sorted(sessions):
            (gain, period) = sessions[sid]
            print('Session {}:'.format(n - sid + 1))
            print("Ultimate gain: {}".format(gain))
            print('Oscillation period (s): {:.3f}'.format(period))
            print()
        print()

if __name__ == '__main__':
    main()
