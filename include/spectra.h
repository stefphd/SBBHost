/*
spectra analysis implementation using fftw and std::vector
*/

#ifndef SPECTRA_H
#define SPECTRA_H

#include <vector>
#include <fftw3.h>
#include <math.h>

/*! \brief A namespace providing wrappers for FFTW library.
    \details Namespace providing wrapper function and utilities for spectral analysis 
    using the FFTW library and the std::vector library.
    \author Stefano Lovato
    \date 2022
*/
namespace spectra {

    //constants
    constexpr int REAL = 0; //!< Position of the real part in fftw_complex type
    constexpr int IMAG = 1; //!< Position of the imag part in fftw_complex type

    /*! \brief Compute the magnitute.
		\details Function to compute the magnitude of a complex vector.
        \param vals A std::vector<fftw_complex> used to compute the magnitude.
		\return A std::vector<double> containing the maginutude.
	*/
    std::vector<double> abs(std::vector<fftw_complex> vals);

    /*! \brief Compute the magnitute squared.
		\details Function to compute the magnitude of a complex vector.
        \param vals A std::vector<fftw_complex> used to compute the magnitude squared.
		\return A std::vector<double> containing the maginutude squared.
	*/
    std::vector<double> abs2(std::vector<fftw_complex> vals);

    /*! \brief Compute the phase in radiants
		\details Function to compute the phase in radiants of a complex vector.
        \param vals A std::vector<fftw_complex> used to compute the phase.
		\return A std::vector<double> containing the phase in radiants.
	*/
    std::vector<double> arg(std::vector<fftw_complex> vals);

    /*! \brief Compute the phase in degrees
		\details Function to compute the phase in degrees of a complex vector.
        \param vals A std::vector<fftw_complex> used to compute the phase.
		\return A std::vector<double> containing the phase in degrees.
	*/
    std::vector<double> argd(std::vector<fftw_complex> vals);

    /*! \brief Compute the polar coordinates.
		\details Function to compute the polar coordinates (magnitude and phase in radians) 
        of a complex vector.
        \param vals A std::vector<fftw_complex> used to compute the phase.
        \param mag A pointer to a std::vector<double> to store the computed magnitude.
        \param phase A pointer to a std::vector<double> to store the computed phase (in radians).
	*/
    void polar(std::vector<fftw_complex> vals, std::vector<double> *mag, std::vector<double> *phase);

    /*! \brief Compute the polar coordinates.
		\details Function to compute the polar coordinates (magnitude and phase in degrees) 
        of a complex vector.
        \param vals A std::vector<fftw_complex> used to compute the phase.
        \param mag A pointer to a std::vector<double> to store the computed magnitude.
        \param phase A pointer to a std::vector<double> to store the computed phase (in degrees).
	*/
    void polard(std::vector<fftw_complex> vals, std::vector<double> *mag, std::vector<double> *phase);

    /*! \brief Compute the bode coordinates.
		\details Function to compute the bode coordinates (decibel and phase in radians) 
        of a complex vector.
        \param vals A std::vector<fftw_complex> used to compute the phase.
        \param mag A pointer to a std::vector<double> to store the computed decibel.
        \param phase A pointer to a std::vector<double> to store the computed phase (in radians).
	*/
    void bode(std::vector<fftw_complex> vals, std::vector<double> *mag, std::vector<double> *phase);

    /*! \brief Compute the bode coordinates.
		\details Function to compute the bode coordinates (decibel and phase in degrees) 
        of a complex vector.
        \param vals A std::vector<fftw_complex> used to compute the phase.
        \param mag A pointer to a std::vector<double> to store the computed decibel.
        \param phase A pointer to a std::vector<double> to store the computed phase (in degrees).
	*/
    void boded(std::vector<fftw_complex> vals, std::vector<double> *mag, std::vector<double> *phase);

    /*! \brief Compute the frequency vector.
		\details Function to compute the frequency vector with given size and maximum frequency.
        \param size The size of the resulting vector.
        \param fmax The maximum frequency.
        \return A std::vector<double> containing `size` frequency from 0 to `fmax`.
	*/
    std::vector<double> frequency(size_t size, double fmax = 0.5);

    /*! \brief Compute the spectra.
		\details Function to compute the complex spectra of a real signal using the discrete-fourier-transform 
        with the FFTW library.
        \param vals A std::vector<double> containing the values of the signal.
        \return A std::vector<fftw_complex> containing the complex spectra of the signal.
	*/
    std::vector<fftw_complex> fft(std::vector<double> vals);

    /*! \brief Compute the power-spectra-density.
		\details Function to compute the power-spectra-density of a real signal using the discrete-fourier-transform 
        with the FFTW library.
        \param vals A std::vector<double> containing the values of the signal.
        \return A A std::vector<double> containing the power-spectra-density of the signal.
	*/
    std::vector<double> psd(std::vector<double> vals);

}

#endif