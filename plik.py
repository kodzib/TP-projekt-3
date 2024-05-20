from vsstudio.Debug.Pybind11module import * #nasza napisana biblioteka
import soundfile as sf # wsparcie do wgrywanie audio
import numpy as np
import imageio.v3 as iio
import os

say_hello(123) #sprawdzamy czy nasza biblioteka dziala

#wgrywanie danych
fixed_path = str(os.path.dirname(os.path.realpath(__file__))).replace("\\", "/")
image =iio.imread(fixed_path + '/obraz.jpg')
audio_data, audio_samplerate = sf.read(fixed_path + '/test_audio_file_1.wav')

#plotowanie
plot_audio(signal_generator('s', 100, audio_samplerate, 1600), fixed_path + "/graph.png")

#generowanie
sf.write(fixed_path + '/output_sin.wav', signal_generator('s', 100, audio_samplerate, 100000), audio_samplerate)
sf.write(fixed_path + '/output_cos.wav', signal_generator('c', 100, audio_samplerate, 100000), audio_samplerate)
sf.write(fixed_path + '/output_square.wav', signal_generator('p', 100, audio_samplerate, 100000), audio_samplerate)
sf.write(fixed_path + '/output_tri.wav', signal_generator('t', 100, audio_samplerate, 100000), audio_samplerate)

#gauss 1d
sf.write(fixed_path + '/output_gauss.wav', filtracja_d(audio_data, 'g', 5), audio_samplerate)

#filtracja 1d
sf.write(fixed_path + '/output_filtracja.wav', filtracja_d(audio_data, 'f', 4), audio_samplerate)

#gauss 2d
iio.imwrite(fixed_path + '/filtered_image_g.jpg', filtracja_img(image, 4, 'g'))

#filtracja 2d
iio.imwrite(fixed_path + '/filtered_image_z.jpg', filtracja_img(image, 4, 'z'))
iio.imwrite(fixed_path + '/filtered_image_c.jpg', filtracja_img(image, 4, 'r'))
iio.imwrite(fixed_path + '/filtered_image_s.jpg', filtracja_img(image, 4, 's'))