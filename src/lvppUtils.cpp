// Copyright 2023 Robert M. Wolff (bob dot wolff 68 at gmail dot com)
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this 
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors 
// may be used to endorse or promote products derived from this software without 
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "lvppUtils.h"

/** @file lvppUtils.cpp
 *  @brief A few useful items for ESP32 use and printing.
 * 
*/

/**
 * @brief Globally available instance of the object to follow most Arduino-based library patterns.
 * 
 */
espSystats ESPSystats;

/**
 * @brief Print a line of pre-formed information to Serial.* or simply via printf()
 * 
 * @param pS Pointer to the message that is to be printed.
 */
inline void printCharPtr(const char* pS) {
#ifdef ESP_PLATFORM
    Serial.println(pS);
#else
    printf("%s\n", pS);
#endif
}

/**
 * @brief Helper funciton to allow printing of a std::string rather than a char*
 * 
 * @param s Reference to a std::string that is to be printed.
 */
inline void printStr(std::string& s) {
    printCharPtr(s.c_str());
}

espSystats::espSystats() {
#ifdef ESP_PLATFORM
    cpuFreqMHz = ESP.getCpuFreqMHz();
    sketchSize = ESP.getSketchSize();
    sketchFreeSpace = ESP.getFreeSketchSpace() - sketchSize;
#else
   cpuFreqMHz = sketchFreeSpace = sketchSize = 0;
#endif
}

void espSystats::takeSample(const char* sampName) {
    recordSample(sampName);
}

void espSystats::takeSampleAndPrintResults(const char* sampName) {
    std::string s = takeSampleAndGetResults(sampName);
    printStr(s);
}

std::string espSystats::takeSampleAndGetResults(const char* sampName) {
    std::stringstream rpt;

#ifdef ESP_PLATFORM
    takeSample(sampName);
    espRec_t rec = samples.back();
    // Now print out the static info and the record info
    rpt << getStaticStats();
    rpt << "ESP Heap Free: " << rec.heapFree;
    rpt << ", Largest Heap Block: " << rec.heapLargestBlock;
    rpt << ", Lowest Heap Free Watermark: " << rec.heapLowWaterMark << std::endl;
#else
    rpt << "ESP Systats: Not running on ESP32 - no stats.\n";
#endif
    return rpt.str();
}

void espSystats::printStaticStats() {
    std::string s = getStaticStats();
    printStr(s);
}

std::string espSystats::getStaticStats() {
std::stringstream rpt;
#ifdef ESP_PLATFORM
    rpt << "ESP32 STATS: CPU Speed(MHz): " << cpuFreqMHz;
    rpt << ", Sketch Size: " << sketchSize;
    rpt << ", Sketch Free Space: " << sketchFreeSpace << std::endl;
#else
    rpt << "ESP Systats: Not running on ESP32 - no stats.\n";
#endif
    return rpt.str();
}

void espSystats::printFullReport() {
    std::string s = getFullReport();
    printStr(s);
}

std::string espSystats::getFullReport() {
    std::stringstream rpt;
#ifdef ESP_PLATFORM
    constexpr int fWid = 10;
    constexpr int tWid = fWid + fWid + fWid + fWid + 36;    // Fields are "|" + space + 7 + space
    rpt << getStaticStats();
    rpt << std::left;       // Left align in fields instead of right by default
    rpt.fill('-');
    rpt << std::setw(tWid) << '-' << std::endl;
    rpt.fill(' ');
    rpt << "| HEAPSIZ | FREE    | LARGEST | MIN     " << std::setw(35) << "| SAMPLE NAME" << "|" << std::endl;
    rpt.fill('-');
    rpt << std::setw(tWid) << '-' << std::endl;
    rpt.fill(' ');
    for(auto it: samples) {
        rpt << "| " << std::setw(fWid-2) << it.heapSize;
        rpt << "| " << std::setw(fWid-2) << it.heapFree;
        rpt << "| " << std::setw(fWid-2) << it.heapLargestBlock;
        rpt << "| " << std::setw(fWid-2) << it.heapLowWaterMark;
        rpt << "| " << std::setw(32) << it.sampleName << " |" << std::endl;
    }
    rpt.fill('-');
    rpt << std::setw(tWid) << '-' << std::endl;
#else
    rpt << "ESP Systats: Not running on ESP32 - no stats.\n";
#endif
    return rpt.str();
}

void espSystats::recordSample(const char* sampName) {
#ifdef ESP_PLATFORM
    espRec_t rec;
    if (samples.size() > 50) {
        Serial.printf("WARNING: espSystats sample count exceeds 50. This is unusal. Current count is: %u\n", samples.size());
    }
    rec.heapFree = ESP.getFreeHeap();
    rec.heapSize = ESP.getHeapSize();
    rec.heapLargestBlock = ESP.getMaxAllocHeap();
    rec.heapLowWaterMark = ESP.getMinFreeHeap();

    strncpy(rec.sampleName, sampName, MAX_SAMPLENAME);
    rec.sampleName[MAX_SAMPLENAME] = 0;     // Guarantee null termination beyond MAX_SAMPLENAME by one character

    samples.push_back(rec);
#endif
}
