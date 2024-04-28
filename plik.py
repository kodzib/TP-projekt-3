from vsstudio.Debug.pybind11module import * #nasza napisana biblioteka
import soundfile as sf # wsparcie do wgrywanie audio
import numpy as np

say_hello() #sprawdzamy czy nasza biblioteka dziala

test_audio_path = 'C:/Users/kbrow/OneDrive/Desktop/projekty/TP projekt 3/TP-projekt-3/test_audio_file_1.wav'

audio_data, audio_samplerate = sf.read(test_audio_path)
plot_audio(audio_data)