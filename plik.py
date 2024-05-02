from vsstudio.Debug.pybind11module import * #nasza napisana biblioteka
import soundfile as sf # wsparcie do wgrywanie audio
import numpy as np
import os

say_hello(123) #sprawdzamy czy nasza biblioteka dziala

fixed_path = str(os.path.dirname(os.path.realpath(__file__))).replace("\\", "/")

audio_data, audio_samplerate = sf.read(fixed_path + '/test_audio_file_1.wav')
#plot_audio(audio_data, fixed_path + "/graph.svg")
#plot_audio(signal_generator('s', 100, audio_samplerate, 1000), fixed_path + "/graph.png")

sf.write(fixed_path + '/output_sin.wav', signal_generator('s', 100, audio_samplerate, 100000), audio_samplerate)
sf.write(fixed_path + '/output_cos.wav', signal_generator('c', 100, audio_samplerate, 100000), audio_samplerate)
sf.write(fixed_path + '/output_square.wav', signal_generator('p', 100, audio_samplerate, 100000), audio_samplerate)
sf.write(fixed_path + '/output_tri.wav', signal_generator('t', 100, audio_samplerate, 100000), audio_samplerate)

sf.write(fixed_path + '/output_gauss.wav', filtracja_d(audio_data, 'g', 3), audio_samplerate)
sf.write(fixed_path + '/test.wav', audio_data, audio_samplerate)