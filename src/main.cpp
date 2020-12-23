/**
 * Signals and Systems Project 2020
 * Objective: Vector Convolution - Convolution of .wav audio files
 *
 * @file main.cpp
 *
 * @author Parmenion Charistos
 * @AEM 3173
 * @date 12/12/2020
 */



/**
 * Including necessary libraries and header files.
 */

#include "AudioFile.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <ctime>

/**
 * Defining shortcuts for paths.
 */

#define SAMPLE "../res/sample_audio.wav"
#define PINK_NOISE "../res/pink_noise.wav"
#define PINK_NOISE_SAMPLE "../out/pinkNoise_sampleAudio.wav"
#define WHITE_NOISE_SAMPLE "../out/whiteNoise_sampleAudio.wav"


/**
 * Convolves two input vectors.
 *
 * @param A : first convolution operand (vector).
 * @param B : second convolution operand (vector).
 * @return result of the convolution of A and B, as a new vector.
 */
std::vector<double> convolve(std::vector<double> &A, std::vector<double> &B) {	//Method for calculating and returning the convolution of two input vectors.

    long length = A.size() + B.size() - 1;
	std::vector<double> C(length, 0);

	for (long x = 0; x < length; x++) {
		unsigned long start = x >= B.size() - 1 ? x - B.size() + 1 : 0;		    //Setting valid bounds for sum operator.
		unsigned long end = x < A.size() - 1 ? x : A.size() - 1;

		for (long u = start; u <= end; u++) {
			C.at(x) += A.at(u) * B.at(x - u);							    //Applying vector convolution formula.
		}
	}

    return C;
}

/**
 * Startup function. Here lie, sequentially, the project's tasks and their implementation. Namely:
 *
 * PART ONE (i) Ask user for a size > 10 for a vector A of random elements. Then, calculate its convolution with a standard vector B.
 * PART TWO (i) Convolve the .wav audio files given (pink_noise.wav , sample_audio.wav) and output the result in pinkNoise_sampleAudio.wav.
 * PART TWO (ii) Create a white noise signal and convolve it with sample_audio.wav. Output the result in whiteNoise_sampleAudio.wav.
 */
int main()
{
	//PART ONE (function for calculation of convolution of two vectors).

	long length;

	do {
		std::cout << "Enter vector size: ";    	//Reading vector size. Should be greater than 10.
        std::cin >> length;
		if (std::cin.fail()) {				    //If the input is non integer, set a default size of 15.
			length = 100;
		}
	} while (length <= 10);

    std::cout << std::endl << "The selected length is " << length << "." << std::endl;

    std::vector<double> A;

    srand(clock());

	for (long i = 0; i < length; i++) {			//Initializing vector A with random values.
		A.push_back(rand());
	}

    std::vector<double> B(5, 0.2);		//Initializing vector B = [1/5, 1/5, 1/5, 1/5, 1/5].

    std::vector<double> C = convolve(A, B);

    std::ofstream myfile;
    myfile.open ("../out/ABconvolution.txt");

    std::cout << "A: ";
    for (int i = 0; i < A.size(); i++) {
        std::cout << A.at(i) << " ";                      //Printing convolution to file.
    }

    std::cout << std::endl;
    std::cout << "B: ";
    for (int i = 0; i < B.size(); i++) {
        std::cout << B.at(i) << " ";
    }

    std::cout << std::endl;
    std::cout << "A*B: ";
    for (int i = 0; i < C.size(); i++) {
        std::cout << C.at(i) << " ";
    }

    std::cout << std::endl;

    for (int i = 0; i < C.size(); i++) {
        myfile << C.at(i) << std::endl;         //Printing convolution to file.
	}
	std::cout << "The first task has been completed. You can find the result of the convolution in AB_convolution.txt of the /out folder." << std::endl;
    myfile.close();



	//PART TWO

	//(i) Convolution calculation for two audio .wav files.

	AudioFile<double> sample_file;											        //Loading audio files via AudioFile library.
	AudioFile<double> pink_noise_file;
	sample_file.AudioFile::load(SAMPLE);
	pink_noise_file.AudioFile::load(PINK_NOISE);


    std::vector<double> Sample = sample_file.samples.at(0);                      //Initializing vectors with files' samples.
    std::vector<double> Pink_Noise = pink_noise_file.samples.at(0);

    std::vector<double> Pink_Noise_Sample = convolve(Sample,Pink_Noise);	    //Calculating convolution of pink noise signal and sample audio.

	AudioFile<double> pink_noise_sample_file;
	pink_noise_sample_file.samples[0] = Pink_Noise_Sample;
	pink_noise_sample_file.save(PINK_NOISE_SAMPLE, AudioFileFormat::Wave);	//Saving convolution in a .wav file.
    std::cout << "The second task has been completed. You can find the resulting pinkNoise_sampleAudio.wav file of the convolution in the /out folder." << std::endl;



    //(ii) Convolution calculation for a made up white noise signal and an audio .wav file.

    std::vector<double> White_Noise;                                                //Defining White Noise vector.
    length = Pink_Noise.size();                                                     //Setting its size equal to the one of the pink noise given before.

    std::random_device ranDev;
    std::mt19937 rand_engine(ranDev());
    std::uniform_real_distribution<double> distro(0, 10);

    for (int i = 0; i < length; i++) {
        White_Noise.push_back(distro(rand_engine));                       //Filling vector with random numbers distributed in the interval [0,1].
    }

    std::vector<double> White_Noise_Sample = convolve(Sample,White_Noise);   //Calculating convolution of white noise signal and sample audio file.

    AudioFile<double> white_noise_sample_file;
    white_noise_sample_file.samples[0] = White_Noise_Sample;
    white_noise_sample_file.save(WHITE_NOISE_SAMPLE,AudioFileFormat::Wave);  //Saving convolution in a .wav file.
    std::cout << "The third task has been completed. You can find the resulting whiteNoise_audioSample.wav of the convolution in the /out folder." << std::endl;

	return 0;
}


