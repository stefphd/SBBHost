#include "spectra.h"

std::vector<double> spectra::abs(std::vector<fftw_complex> vals) {
    std::vector<double> out(vals.size());
    for (int i = 0; i < vals.size(); ++i) out[i] = sqrt((vals[i][spectra::REAL])*(vals[i][spectra::REAL]) + (vals[i][spectra::IMAG])*(vals[i][spectra::IMAG]));
    return out;
}

std::vector<double> spectra::abs2(std::vector<fftw_complex> vals) {
    std::vector<double> out(vals.size());
    for (int i = 0; i < vals.size(); ++i) out[i] = ((vals[i][spectra::REAL])*(vals[i][spectra::REAL]) + (vals[i][spectra::IMAG])*(vals[i][spectra::IMAG]));
    return out;
}

std::vector<double> spectra::arg(std::vector<fftw_complex> vals) {
    std::vector<double> out(vals.size());
    for (int i = 0; i < vals.size(); ++i) out[i] = atan2((vals[i][spectra::IMAG]), (vals[i][spectra::REAL]));
    return out;
}

std::vector<double> spectra::argd(std::vector<fftw_complex> vals) {
    std::vector<double> out(vals.size());
    for (int i = 0; i < vals.size(); ++i) out[i] = atan2((vals[i][spectra::IMAG]), (vals[i][spectra::REAL]))*180.0/3.14159265358979323846;
    return out;
}

void spectra::polar(std::vector<fftw_complex> vals, std::vector<double> * mag, std::vector<double> * phase) {
    for (int i = 0; i < vals.size(); ++i) {
        mag->push_back(sqrt((vals[i][spectra::REAL])*(vals[i][spectra::REAL]) + (vals[i][spectra::IMAG])*(vals[i][spectra::IMAG])));
        phase->push_back(atan2((vals[i][spectra::IMAG]), (vals[i][spectra::REAL])));
    }
}

void spectra::polard(std::vector<fftw_complex> vals, std::vector<double> * mag, std::vector<double> * phase) {
    for (int i = 0; i < vals.size(); ++i) {
        mag->push_back(sqrt((vals[i][spectra::REAL])*(vals[i][spectra::REAL]) + (vals[i][spectra::IMAG])*(vals[i][spectra::IMAG])));
        phase->push_back(atan2((vals[i][spectra::IMAG]), (vals[i][spectra::REAL]))*180.0/3.14159265358979323846);
    }
}

void spectra::bode(std::vector<fftw_complex> vals, std::vector<double> * mag, std::vector<double> * phase) {
    for (int i = 0; i < vals.size(); ++i) {
        mag->push_back(sqrt((vals[i][spectra::REAL])*(vals[i][spectra::REAL]) + (vals[i][spectra::IMAG])*(vals[i][spectra::IMAG])));
        phase->push_back(atan2((vals[i][spectra::IMAG]), (vals[i][spectra::REAL])));
    }
}

void spectra::boded(std::vector<fftw_complex> vals, std::vector<double> * mag, std::vector<double> * phase) {
    for (int i = 0; i < vals.size(); ++i) {
        mag->push_back(20*log10(sqrt((vals[i][spectra::REAL])*(vals[i][spectra::REAL]) + (vals[i][spectra::IMAG])*(vals[i][spectra::IMAG]))));
        phase->push_back(atan2((vals[i][spectra::IMAG]), (vals[i][spectra::REAL]))*180.0/3.14159265358979323846);
    }
}

std::vector<fftw_complex> spectra::fft(std::vector<double> vals) {
    std::vector<fftw_complex> in(vals.size());
    std::vector<fftw_complex> out(vals.size());
    for (int i = 0; i < vals.size(); ++i) {
        in[i][spectra::REAL] = vals[i];
        in[i][spectra::IMAG] = 0;
    }
    fftw_plan plan = fftw_plan_dft_1d(vals.size(), in.data(), out.data(), FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    std::vector<fftw_complex> out2((out.size()-1)/2);
    for (int i = 0; i < out2.size(); ++i) {
        out2[i][spectra::REAL] = out[i][spectra::REAL]/vals.size();
        out2[i][spectra::IMAG] = out[i][spectra::IMAG]/vals.size();   
    }
    return out2;
}

std::vector<double> spectra::psd(std::vector<double> vals) {
    return abs2(fft(vals));
}

std::vector<double> spectra::frequency(size_t size, double fmax) {
    std::vector<double> freq((size-1)/2);
    for (int i = 0; i <  freq.size(); ++i) freq[i] = double(i)/double(freq.size() - 1)*fmax;
    return freq;
}