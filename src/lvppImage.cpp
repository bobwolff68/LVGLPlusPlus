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
#include "lvpp.h"

/** @file lvppImage.cpp
 * @brief Implementation for supporting display of an image via the lv_img_dsc_t typedef from LVGL.
*/

lvppImage::lvppImage(const char* fName, lv_obj_t* parent) : lvppBase(fName, "IMAGE") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_img_create(objParent));

    pImage = nullptr;
    deferred_h = deferred_w = -1;
    noRotation = false;
}

lvppImage::~lvppImage() {

}

void lvppImage::setImage(const lv_img_dsc_t* pImg) {
    pImage = pImg;
    lv_img_set_src(obj, pImage);

    // If the user calls setSize() _prior_ to setImage, we'll patch things up.
    if (deferred_h != -1) {
        setSize(deferred_w, deferred_h);
        deferred_h = deferred_w = -1;
    }

    if (pImage->header.cf != LV_IMG_CF_TRUE_COLOR) {
        noRotation = true;
    }
    else {
        noRotation = false;
    }
}

void lvppImage::setSize(lv_coord_t width, lv_coord_t height) {
    if (pImage) {
        if (width != pImage->header.w || height != pImage->header.h) {
            // Need to zoom the image. 
            // Let's figure out what % zoom this width and height are.
            // Then we'll take the smallest % and use that for the zoom.
            // Lastly - we can only zoom 200% so bound it there.
            uint16_t w_fact, h_fact;
            w_fact = h_fact = 100;  // Starting point

            if (width != pImage->header.w) {
                w_fact = 100 * width / pImage->header.w;
                if (w_fact > 200)
                    w_fact = 200;
            }

            if (height != pImage->header.h) {
                h_fact = 100 * height / pImage->header.h;
                if (h_fact > 200)
                    h_fact = 200;
            }

            uint16_t zFactor = std::min(w_fact, h_fact) * 256 / 100;
printf(":%s:lvppImage::setSize() - w_fact=%u, h_fact=%u, zFactor=%u\n", whoAmI(), w_fact, h_fact, zFactor);
            lv_img_set_zoom(obj, zFactor);
        }
        else {
           lvppBase::setSize(width, height);
        }
    }
    else {
        // Do the re-size once the user sets up an image to use for zoom.
        deferred_w = width;
        deferred_h = height;
    }
}

void lvppImage::setRotation(int16_t rotTenthsOfDegrees) {
    if (noRotation) {
        LV_LOG_WARN("lvppImage::setRotation - not allowed for images which are not LV_IMG_CF_TRUE_COLOR\n");
        return;
    }

    lv_img_set_angle(obj, rotTenthsOfDegrees);
}

void lvppImage::setPivotPoint(lv_coord_t xPivot, lv_coord_t yPivot) {
    lv_img_set_pivot(obj, xPivot, yPivot);
}

