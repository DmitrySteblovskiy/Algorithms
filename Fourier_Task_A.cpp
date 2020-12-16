#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <cmath>
#include <complex.h>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <functional>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <vector>


    // Структура, описывающая заголовок WAV файла.
struct WAVHEADER {
  // WAV-формат начинается с RIFF-заголовка:
  // Содержит символы "RIFF" в ASCII кодировке
  // (0x52494646 в big-endian представлении)
    char chunkId[4];
  // 36 + subchunk2Size, или более точно:
  // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
  // Это оставшийся размер цепочки, начиная с этой позиции.
  // Иначе говоря, это размер файла - 8, то есть,
  // исключены поля chunkId и chunkSize.
    unsigned int chunkSize;
  // Содержит символы "WAVE"
  // (0x57415645 в big-endian представлении)
    char format[4];
  // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
  // Подцепочка "fmt " описывает формат звуковых данных:
  // Содержит символы "fmt "
  // (0x666d7420 в big-endian представлении)
    char subchunk1Id[4];
  // 16 для формата PCM.
  // Это оставшийся размер подцепочки, начиная с этой позиции.
    unsigned int subchunk1Size;
  // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
  // Для PCM = 1 (то есть, Линейное квантование).
  // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    unsigned short audioFormat;
  // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    unsigned short numChannels;
  // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    unsigned int sampleRate;
  // sampleRate * numChannels * bitsPerSample/8
    unsigned int byteRate;
  // numChannels * bitsPerSample/8
  // Количество байт для одного сэмпла, включая все каналы.
    unsigned short blockAlign;
  // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    unsigned short bitsPerSample;
  // Подцепочка "data" содержит аудио-данные и их размер.
  // Содержит символы "data"
  // (0x64617461 в big-endian представлении)
    char subchunk2Id[4];
  // numSamples * numChannels * bitsPerSample/8
  // Количество байт в области данных.
    unsigned int subchunk2Size;
  // Далее следуют непосредственно Wav данные.
};

typedef std::complex<double> base;
void FFT(vector<base> &factor) {
    const double M_PI = 3.14159265359;
	  size_t n = factor.size();
	  if (n == 1)  {
        return;
    }
	  std::vector<base> vec_first (n / 2),  vec_second (n / 2);

	  for (int i = 0, j = 0; i < n; i += 2, ++j) {
		    vec_first[j] = factor[i];
		    vec_second[j] = factor[i+1];
	  }
	  fft (vec_first);
	  fft (vec_second);

	  double ang = 2*M_PI/n * (0 ? -1 : 1);
	  base w(1),  w_angle (cos(ang), sin(ang));

	  for (int i = 0; i < n / 2; ++i) {
		    factor[i] = vec_first[i] + w * vec_second[i];
		    factor[i + n / 2] = vec_first[i] - w * vec_second[i];
			  factor[i] /= 2,  factor[i + n / 2] /= 2;

		    w *= w_angle;
	  }
}

typedef std::complex<double> base;
void FFTReverse(vector<base> &factor) {
	  size_t n = factor.size();
	  if (n == 1)  {
        return;
    }
	  std::vector<base> vec_first (n / 2),  vec_second (n / 2);

	  for (int i = 0, j = 0; i < n; i += 2, ++j) {
		    vec_first[j] = factor[i];
		    vec_second[j] = factor[i+1];
	  }
	  fft (vec_first);
	  fft (vec_second);

	  double ang = 2*M_PI/n * (1 ? -1 : 1);
	  base w(1),  w_angle (cos(ang), sin(ang));

	  for (int i = 0; i < n / 2; ++i) {
		    factor[i] = vec_first[i] + w * vec_second[i];
		    factor[i + n/2] = vec_first[i] - w * vec_second[i];
			  factor[i] /= 2,  factor[i + n / 2] /= 2;

		    w *= w_angle;
	  }
}

void Nullification(std::vector<double> &factor) {   // обнуляем 80% коэфов
    for (size_t i = std::floor(factor.size() / 5); i < factor.size(); ++i) {
        factor[i] = 0;
    }
}

char* NewWavFile (std::vector<double> &factor, WAVHEADER new_header, char* new_wav) {}

    char curr;

    for (size_t i = 0; i < static_cast<int>(factor.size()); ++i) {
        curr = static_cast<char>(factor[i]);
        new_wav[i] = curr;
    }
    return new_wav;
}


int main() {
    FILE* file = fopen("speech.wav", "rb");
    if (!file) {
        std::cout << "Failed open file";
        return 0;
    }

    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, file);

    // Выводим полученные данные
    std::cout << header.chunkId[0] << header.chunkId[1] << header.chunkId[2] << header.chunkId[3] << std::endl;
    printf("Chunk size: %d\n", header.chunkSize);
    std::cout << header.format[0] << header.format[1] << header.format[2] << header.format[3] << std::endl;
    std::cout << header.subchunk1Id[0] << header.subchunk1Id[1] << header.subchunk1Id[2] << header.subchunk1Id[3] << std::endl;
    printf("SubChunkId1: %d\n", header.subchunk1Size);
    printf("Audio format: %d\n", header.audioFormat);
    printf("Channels: %d\n", header.numChannels);
    printf("Sample rate: %d\n", header.sampleRate);
    printf("Bits per sample: %d\n", header.bitsPerSample);
    std::cout << header.subchunk2Id[0] << header.subchunk2Id[1] << header.subchunk2Id[2] << header.subchunk2Id[3] << std::endl;

    // 
    float fDurationSeconds = 1.f * header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate;
    int iDurationMinutes = static_cast<int>(std::floor(fDurationSeconds) / 60);
    fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);
    printf("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);

    fclose(file);
    file = fopen("speech.wav", "rb");

    char* data = new char[header.subchunk2Size];
    fread(data, header.subchunk2Size, 1, file);

    std::vector<double> factor;
    std::vector<double> inds;

    for (int i = 0; i < static_cast<int>(header.subchunk2Size); ++i) {
        factor.push_back(data[i]);
    }

    unsigned int size_ = 1;

    while (size_ < header.subchunk2Size) {
        size_ *= 2;
    }

    factor.resize(size_);

    FFT(factor);

    Nullification(factor); // обнуление

    FFTReverse(factor);

    WAVHEADER new_header = header;

    new_header.subchunk2Size = factor.size();
  
    char *new_wav = new char[new_header.subchunk2Size];
    new_wav = NewWavFile(factor, header, new_wav);

    FILE* new_file = fopen("ans.wav", "w");

    fwrite(&new_header, sizeof(new_header), 1, new_file);

    fwrite(new_wav, 1, new_header.subchunk2Size, new_file);

    std::cout << "Data is successfully loaded." << "\n";

    delete[] data;
    delete[] new_wav;

    fclose(file);
    //system("pause");
}