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

lvppCanvas::lvppCanvas(const char* fName, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_obj_t* parent) 
    : lvppBase(fName, "CANVAS") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_canvas_create(objParent));

#if 0
    pBuffer = (lv_color_t*)malloc(sizeof(lv_color_t)*LV_CANVAS_BUF_SIZE_TRUE_COLOR(w, h));
    assert(pBuffer);

    lv_canvas_set_buffer(obj, pBuffer, w, h, LV_IMG_CF_TRUE_COLOR);
    align(LV_ALIGN_TOP_LEFT, x, y);
#else
    pBuffer = (lv_color_t*)malloc(sizeof(lv_color_t)*LV_CANVAS_BUF_SIZE_INDEXED_4BIT(w, h));
    assert(pBuffer);

    lv_canvas_set_buffer(obj, pBuffer, w, h, LV_IMG_CF_INDEXED_4BIT);
    align(LV_ALIGN_TOP_LEFT, x, y);

    lv_canvas_set_palette(obj, 0, lv_color_black());
    lv_canvas_set_palette(obj, 1, lv_palette_main(LV_PALETTE_GREY));
    lv_canvas_set_palette(obj, 2, lv_color_white());
#endif

    pDscLabel = nullptr;
    pDscLine  = nullptr;
    pDscRect  = nullptr;
}

lvppCanvas::~lvppCanvas() {
    if (pBuffer) {
        delete pBuffer;
        pBuffer = nullptr;
    }
    if (pDscLabel) {
        delete pDscLabel;
        pDscLabel = nullptr;
    }
    if (pDscLine) {
        delete pDscLine;
        pDscLine = nullptr;
    }
    if (pDscRect) {
        delete pDscRect;
        pDscRect = nullptr;
    }
}

void lvppCanvas::setbgColor(lv_color_t bgColor, lv_opa_t opacity) {
    lv_canvas_fill_bg(obj, bgColor, LV_OPA_COVER);
}

void lvppCanvas::drawRect(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, 
        lv_color_t color, uint16_t radius, lv_opa_t opa) {

    if (!pDscRect) {
        pDscRect = new lv_draw_rect_dsc_t;
        lv_draw_rect_dsc_init(pDscRect);
    }

    pDscRect->radius = radius;
    pDscRect->bg_opa = opa;
    pDscRect->bg_color = color;
//    rect_dsc.bg_grad.dir = LV_GRAD_DIR_HOR;
//    rect_dsc.bg_grad.stops[0].color = lv_palette_main(LV_PALETTE_RED);
//    rect_dsc.bg_grad.stops[1].color = lv_palette_main(LV_PALETTE_BLUE);
    pDscRect->border_width = 2;
    pDscRect->border_opa = LV_OPA_100;
#if 0
    pDscRect->border_color = lv_color_black();
#else
    lv_color_t b;
    b.full = 0;
    pDscRect->border_color = b;
#endif
    lv_canvas_draw_rect(obj, x, y, w, h, pDscRect);
}

void lvppCanvas::drawLine(lv_coord_t x1, lv_coord_t y1, lv_coord_t x2, lv_coord_t y2, lv_coord_t width, lv_color_t color) {

    if (!pDscLine) {
        pDscLine = new lv_draw_line_dsc_t;
        lv_draw_line_dsc_init(pDscLine);
    }

    pDscLine->width = width;
    pDscLine->color = color;

    twoPoints[0].x = x1;
    twoPoints[0].y = y1;
    twoPoints[1].x = x2;
    twoPoints[1].y = y2;

    lv_canvas_draw_line(obj, twoPoints, 2, pDscLine);
}

void lvppCanvas::drawLabel(lv_coord_t x, lv_coord_t y, lv_coord_t maxW, lv_color_t color, const char* pText) {
    if (!pText)
        return;

    if (!pDscLabel) {
        pDscLabel = new lv_draw_label_dsc_t;
        lv_draw_label_dsc_init(pDscLabel);
    }

    pDscLabel->color = color;

    lv_canvas_draw_text(obj, x, y, maxW, pDscLabel, pText);
}
