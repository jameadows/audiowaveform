//
// Created by jam on 4/16/22.
//
#include "SndFileAudioFileReader.h"
#include "AudioProcessor.h"
#include "Error.h"
#include "FileUtil.h"
#include "Log.h"
#include "ProgressReporter.h"

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include "KajamAudioFileReader.h"

static void showInfo(std::ostream &stream, const SF_INFO &info) {
    stream << "Frames: " << info.frames
           << "\nSample rate: " << info.samplerate << " Hz"
           << "\nChannels: " << info.channels
           << "\nFormat: 0x" << std::hex << info.format << std::dec
           << "\nSections: " << info.sections
           << "\nSeekable: " << (info.seekable ? "yes" : "no") << '\n';
}

KajamAudioFileReader::KajamAudioFileReader() {
}

bool KajamAudioFileReader::open(const char *input_filename, bool show_info) {
    assert(input_file_ == nullptr);

    // Set up the info structure for the expected format

    info_.format = SF_FORMAT_RAW | SF_ENDIAN_LITTLE | SF_FORMAT_FLOAT;
    info_.channels = 1;
    info_.samplerate = 48000;

    input_file_ = sf_open(input_filename, SFM_READ, &info_);

    if (input_file_ == nullptr) {
        log(Error) << "Failed to read file: " << input_filename << '\n'
                   << sf_strerror(nullptr) << '\n';

        return false;
    }

    log(Info) << "Input file: "
              << FileUtil::getInputFilename(input_filename) << '\n';

    if (show_info) {
        showInfo(log(Info), info_);
    }

    return true;
}
