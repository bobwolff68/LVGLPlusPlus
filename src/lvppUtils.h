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
#pragma once

#ifdef ESP_PLATFORM
#include "Arduino.h"
#endif

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

/** @file lvppUtils.h
 *  @brief A few useful items for ESP32 use
 * 
*/

constexpr uint16_t MAX_SAMPLENAME = 32;

/**
 * @brief Struct which holds the data taken in each sample for espSystats class.
 *        A typedef of espRec_t is created for ease of use as well.
 * 
 */
typedef struct espRec {
    uint32_t heapFree;
    uint32_t heapSize;
    uint32_t heapLargestBlock;
    uint32_t heapLowWaterMark;
    char sampleName[MAX_SAMPLENAME+1];
} espRec_t;

/**
 * @brief Data gather and report on ESP32 system stats - mostly memory heap.
 * 
 * For quite a while I've used the ESP.* utility functions once in a while to print out
 * key heap and sketch metrics when I'm scratching my head about free space and such.
 * This class advances that notion by allowing the user to call ESPSystats.takeSample()
 * and pass an argument which describes the point in the lifecycle in descriptive terms.
 * These 'takeSample' items are collected by the class and are available at any time
 * to be printed by simply calling ESPSystats.printFullReport().
 * The resulting output gives the user a table of memory and other stat information
 * in chronological order from each takeSample() completed. It is a super handy way to
 * understand what objects or portions of your code are impacting usage.
 * An example of the output from printFullReport() follows:
 * 
 *     ESP32 STATS: CPU Speed(MHz): 160, Sketch Size: 874400, Sketch Free Space: 436320
 *     ----------------------------------------------------------------------------
 *     | HEAPSIZ | FREE    | LARGEST | MIN     | SAMPLE NAME                      |
 *     ----------------------------------------------------------------------------
 *     | 312060  | 286928  | 110580  | 281356  | SETUP-pre-tft                    |
 *     | 311996  | 286628  | 110580  | 281056  | SETUP-pre-lvgl                   |
 *     | 310940  | 282516  | 110580  | 276908  | SETUP-post-lvgl                  |
 *     | 310940  | 282412  | 110580  | 276684  | SETUP-pre-widgets                |
 *     | 310068  | 191452  | 69620   | 185880  | Splash-start                     |
 *     | 306908  | 173676  | 64500   | 173580  | SETUP-end                        |
 *     | 306892  | 173708  | 64500   | 171492  | Splash-end                       |
 *     ----------------------------------------------------------------------------
 * 
 * _Note: Class is functional on ESP32, but on a simulated environment simply prints
 *        (via printf()) a note that such stats are unavailable._
 */


class espSystats {
public:
    espSystats();
/**
 * @brief Take a data sample immediately and memorialize it with the name passed in the argument.
 * 
 * @param sampName A simple name to mark in descriptive terms when the sample was taken
 *                 in the lifecycle of the program calling it.
 */
    void takeSample(const char* sampName);      // Records current stats.
/**
 * @brief Take a sample (via takeSample()) and then print (in non-tabular form) the results.
 * 
 * @param sampName A simple name to mark in descriptive terms when the sample was taken
 *                 in the lifecycle of the program calling it.
 */
    void takeSampleAndPrintResults(const char* sampName);
/**
 * @brief Take a sample and return the results as a std::string.
 * 
 * @param sampName A simple name to mark in descriptive terms when the sample was taken
 *                 in the lifecycle of the program calling it.
 * @return std::string Rather than printing the results - return the results as a std::string.
 */
    std::string takeSampleAndGetResults(const char* sampName);
/**
 * @brief Prints the non-record-based statistics only.
 * 
 * There are a few items which do not change on a per-sample basis. These are captured
 * at instantiation time and are available to be printed here. These are: cpuFreqMHz,
 * sketchFreeSpace, and sketchSize.
 * 
 */
    void printStaticStats();
/**
 * @brief Get the non-record-based statistics only and return them in a std::string.
 *        See printStaticStats() for more detail.
 * 
 * @return std::string Rather than printing, return results in a std::string to the caller.
 */
    std::string getStaticStats();
/**
 * @brief This is the _main event_ for results along with getFullReport(). This prints
 *        the static stats as well as a full table of all records taken via the *Sample() functions.
 * 
 * The typical usage pattern is to utilize takeSample() from multiple places in your project -
 * especially during startup where objects are created and graphics are set up into memory. Then,
 * at a point when startup is relatively stable, this function is called to get a single final
 * report to view.
 * 
 * This function can be called multiple times, however there is no facility to clear out prior
 * records. Any newly taken samples will be appended to the report in subsequent calls for
 * a full report.
 * 
 * A sample output report follows:
 * 
 *     ESP32 STATS: CPU Speed(MHz): 160, Sketch Size: 874400, Sketch Free Space: 436320
 *     ----------------------------------------------------------------------------
 *     | HEAPSIZ | FREE    | LARGEST | MIN     | SAMPLE NAME                      |
 *     ----------------------------------------------------------------------------
 *     | 312060  | 286928  | 110580  | 281356  | SETUP-pre-tft                    |
 *     | 311996  | 286628  | 110580  | 281056  | SETUP-pre-lvgl                   |
 *     | 310940  | 282516  | 110580  | 276908  | SETUP-post-lvgl                  |
 *     | 310940  | 282412  | 110580  | 276684  | SETUP-pre-widgets                |
 *     | 310068  | 191452  | 69620   | 185880  | Splash-start                     |
 *     | 306908  | 173676  | 64500   | 173580  | SETUP-end                        |
 *     | 306892  | 173708  | 64500   | 171492  | Splash-end                       |
 *     ----------------------------------------------------------------------------
 * 
 */
    void printFullReport();
/**
 * @brief Same functionality as printFullReport() but returns the result in std::string
 *        instead of printing the results out.
 * 
 * @return std::string Full report returned as a std::string object.
 */
    std::string getFullReport();
protected:
/**
 * @brief Internal worker-bee which actually takes the sample and places it in a record.
 * 
 * @param sampName A simple name to mark in descriptive terms when the sample was taken
 *                 in the lifecycle of the program calling it.
 */
    void recordSample(const char* sampName); 
    uint32_t cpuFreqMHz;            ///< The processor frequency at the time of instantiation.
    uint32_t sketchFreeSpace;       ///< Number of bytes remaining in the sketch/program area.
    uint32_t sketchSize;            ///< Number of bytes taken by the current sketch/program.
    std::vector<espRec_t> samples;  ///< std::vector which stores each sample in the order taken.
};

/**
 * @brief Globally available instance of the object to follow most Arduino-based library patterns.
 * 
 */
extern espSystats ESPSystats;
