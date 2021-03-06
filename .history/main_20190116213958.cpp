extern "C"{
	#include "fftsg.c"
}
#include "overlapAdd.hpp"
#include "wavFile/wavFile.cpp"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>

int main(){
	using namespace std;

	cout << "ファイル名 (please input file name) >" << flush;
	std::string fn;
	cin >> fn;

	// wavファイル読み込み（16bit・モノラル・リニアPCMのみ対応）
	WavFile wav;
	if(!wav.load(fn)){
		cout << "error at file input" << endl;
		return 0;
	}

	// fft用のバッファ（fftsgが使う）
	const int n = 4096;
	int ip[n];
	double w[n*8];

	// 入力波に窓関数をかけて、再合成。
	overlapAdd<double>(
		wav.data.begin(), wav.data.end(),	// 処理範囲
		n,									// 窓幅
		WindowFunction::sine,				// 窓関数の種類
		[&](auto first, auto last){			// 窓関数がかかった波形への処理内容

			// スペクトルの取得
			vector<double> resultOfRdft(first, last);
			vector<double> originalFrequencySpectrum(resultOfRdft.size()/2 + 1, 0);
			rdft(n, -1, resultOfRdft.data(), ip, w);

			// c.f. fftsg.c line from 96 to 99
			originalFrequencySpectrum[0] = resultOfRdft[0];
			for(int i=1; i<originalFrequencySpectrum.size(); i++){
				originalFrequencySpectrum[i] = pow(resultOfRdft[i*2], 2) + pow(resultOfRdft[i*2+1], 2);
			}

			// 取得したスペクトルにLPFをかける
			vector<double> frequencySpectrum(originalFrequencySpectrum.begin(), originalFrequencySpectrum.end());
			rdft(frequencySpectrum.size(), -1, frequencySpectrum.data(), ip, w);
			frequencySpectrum[1] = 0;
			for(int i=frequencySpectrum.size()/2; i<frequencySpectrum.size(); i++){
				frequencySpectrum[i] = 0;
			}
			rdft(frequencySpectrum.size(), 1, frequencySpectrum.data(), ip, w);

			copy(a.begin(),a.end(),first);
		},
		WindowFunction::sine
	);

	wav.save(fn+"-out.wav");
}