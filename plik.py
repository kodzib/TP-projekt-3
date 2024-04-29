from vsstudio.Debug.pybind11module import * #nasza napisana biblioteka
import soundfile as sf # wsparcie do wgrywanie audio
import numpy as np

say_hello(123) #sprawdzamy czy nasza biblioteka dziala

test_audio_path = 'C:/Users/witfo/Desktop/Tp projekt 3/TP-projekt-3/test_audio_file_1.wav'
output_file_path = 'C:/Users/witfo/Desktop/Tp projekt 3/TP-projekt-3/output.wav'

audio_data, audio_samplerate = sf.read(test_audio_path)
#plot_audio(audio_data)

sf.write(output_file_path, signal_generator('s', 0.01, 100000), audio_samplerate)