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

/** @file lvppCanvas.cpp
 * @brief Implementation of both Canvas types - full color and indexed color.
*/

lvppCanvasIndexed::lvppCanvasIndexed(const char* fName, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, uint8_t colorDepth, lv_color_t* providedBuffer, lv_obj_t* parent) 
    : lvppBase(fName, "CANVASINDEXED") {
    lv_img_cf_t cfType;
    int bufSize;

    width = w;
    height = h;

    objParent = parent ? parent : lv_scr_act();
    createObj(lv_canvas_create(objParent));

    // All pBuffer allocation (or lack of) is done in the derived classes.
    pBuffer=nullptr;

    switch(colorDepth) {
        case 1:
            cfType = LV_IMG_CF_INDEXED_1BIT;
            maxColorIndexesAllowed = 2;
            bufSize = sizeof(lv_color_t)*LV_CANVAS_BUF_SIZE_INDEXED_1BIT(w, h);
            break;
        case 2:
            cfType = LV_IMG_CF_INDEXED_2BIT;
            maxColorIndexesAllowed = 4;
            bufSize = sizeof(lv_color_t)*LV_CANVAS_BUF_SIZE_INDEXED_2BIT(w, h);
            break;
        case 4:
            cfType = LV_IMG_CF_INDEXED_4BIT;
            maxColorIndexesAllowed = 16;
            bufSize = sizeof(lv_color_t)*LV_CANVAS_BUF_SIZE_INDEXED_4BIT(w, h);
            break;
        case 8:
            cfType = LV_IMG_CF_INDEXED_8BIT;
            maxColorIndexesAllowed = 256;
            bufSize = sizeof(lv_color_t)*LV_CANVAS_BUF_SIZE_INDEXED_8BIT(w, h);
            break;
        default:
            LV_LOG_ERROR("lvppCanvasIndexed: colorDepth must be one of 1,2,4,8.");
            throw;
            break;
    }

    // If a buffer is provided, use it but don't assign pBuffer so it doesn't get deallocated.
    if (providedBuffer) {
        lv_canvas_set_buffer(obj, providedBuffer, w, h, cfType);
    }
    else {
        pBuffer = (lv_color_t*)malloc(bufSize);
        if (!pBuffer)
            printf("ERROR: Unable to allocate lvppCanvas buffer of size: %d\n", bufSize);
            
        assert(pBuffer);

        lv_canvas_set_buffer(obj, pBuffer, w, h, cfType);
    }

    align(LV_ALIGN_TOP_LEFT, x, y);

    clearColorIndex();
}

lvppCanvasIndexed::~lvppCanvasIndexed() {
    if (pBuffer) {
        free(pBuffer);
    }
}

//
// Map a new color into the color index for later use.
//
bool lvppCanvasIndexed::addColorToIndex(lv_color_t col) {
    if (colorToIndex.count(col.full))
        return true; // We've already got this index color

    if (colorIndexesUsed < maxColorIndexesAllowed) {
        colorToIndex[col.full] = colorIndexesUsed;
        lv_canvas_set_palette(obj, colorIndexesUsed, col);
        colorIndexesUsed++;
        return true;
    }
    else
        return false;
}

bool lvppCanvasIndexed::addPaletteToIndex(lv_palette_t pal) {
    lv_color_t cc;
    // Need to have space for 10 new color indexes.
    if (colorIndexesUsed < maxColorIndexesAllowed-10) {
        // Darker shades first
        for (int i=4;i>=1;i--) {
            cc = lv_palette_darken(pal, i);
            addColorToIndex(cc);
        }
        // Main central color
        cc = lv_palette_main(pal);
        addColorToIndex(cc);
        // Lighter shades next
        for (int i=1;i<=5;i++) {
            cc = lv_palette_lighten(pal, i);
            addColorToIndex(cc);
        }
        return true;
    }
    else
        return false;
}

void lvppCanvasIndexed::clearColorIndex() {
    colorIndexesUsed=0;
    colorToIndex.clear();
}

bool lvppCanvasIndexed::getIndexFromColor(lv_color_t col, lv_color_t& ind) {
    if (colorToIndex.count(col.full)) {
        ind.full = colorToIndex[col.full];
        return true;
    }
    else {
        ind.full = 0;
        return false;
    }
}

void lvppCanvasIndexed::setbgColor(lv_color_t bgColor) {
    lv_color_t colInd;
    if (getIndexFromColor(bgColor, colInd)) {
        setbgColorByIndex(colInd);
    }
    else {
        LV_LOG_WARN("setbgColor: color not found in palette.");
    }
}

void lvppCanvasIndexed::setbgColorByIndex(lv_color_t bgColorIndex) {
    if (bgColorIndex.full>=colorIndexesUsed) {
        LV_LOG_WARN("setbgColorByIndex: index out of palette range.");
        return;
    }

    lv_canvas_fill_bg(obj, bgColorIndex, LV_OPA_COVER);
}

void lvppCanvasIndexed::drawPixel(lv_coord_t x, lv_coord_t y, lv_color_t color) {
    lv_color_t colInd;
    if (getIndexFromColor(color, colInd)) {
        drawPixelByIndex(x, y, colInd);
    }
    else {
        LV_LOG_WARN("drawPixel: color not found in palette.");
    }
}

void lvppCanvasIndexed::drawPixelByIndex(lv_coord_t x, lv_coord_t y, lv_color_t colorIndex) {
    if (colorIndex.full>=colorIndexesUsed) {
        LV_LOG_WARN("drawPixelByIndex: index out of palette range.");
        return;
    }

    lv_canvas_set_px_color(obj, x, y, colorIndex);
}

void lvppCanvasIndexed::drawLineVert(lv_coord_t x1, lv_coord_t y1, lv_coord_t h, lv_color_t color) {
    lv_color_t colInd;
    if (getIndexFromColor(color, colInd)) {
        drawLineVertByIndex(x1, y1, h, colInd);
    }
    else {
        LV_LOG_WARN("drawLineVert: color not found in palette.");
    }
}

void lvppCanvasIndexed::drawLineVertByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t h, lv_color_t indexCol) {
    if (indexCol.full>=colorIndexesUsed) {
        LV_LOG_WARN("drawLineVertByIndex: index out of palette range.");
        return;
    }

    for (lv_coord_t y=y1; y<y1+h; y++) {
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x1, y, indexCol);
    }

    lv_obj_invalidate(obj);
}

void lvppCanvasIndexed::drawLineHoriz(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_color_t color) {
    lv_color_t colInd;
    if (getIndexFromColor(color, colInd)) {
        drawLineHorizByIndex(x1, y1, w, colInd);
    }
    else {
        LV_LOG_WARN("drawLineHoriz: color not found in palette.");
    }
}

void lvppCanvasIndexed::drawLineHorizByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_color_t indexCol) {
    if (indexCol.full>=colorIndexesUsed) {
        LV_LOG_WARN("drawLineHorizByIndex: index out of palette range.");
        return;
    }

    for (lv_coord_t x=x1; x<x1+w; x++) {
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x, y1, indexCol);
    }

    lv_obj_invalidate(obj);
}

void lvppCanvasIndexed::drawRectWithoutFill(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColor) {
    lv_color_t colInd;
    if (getIndexFromColor(borderColor, colInd)) {
        drawRectWithoutFillByIndex(x1, y1, w, h, colInd);
    }
    else {
        LV_LOG_WARN("drawRectWithoutFill: color not found in palette.");
    }
}

void lvppCanvasIndexed::drawRectWithoutFillByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColorInd) {
    if (borderColorInd.full>=colorIndexesUsed) {
        LV_LOG_WARN("drawRectWithoutFillByIndex: index out of palette range.");
        return;
    }

    for (lv_coord_t x=x1 ; x<x1+w; x++)
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x, y1, borderColorInd);

    for (lv_coord_t y=y1; y<y1+h; y++)
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x1, y, borderColorInd);

    for (lv_coord_t y=y1; y<y1+h; y++)
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x1+w, y, borderColorInd);

    for (lv_coord_t x=x1 ; x<x1+w; x++)
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x, y1+h, borderColorInd);

    lv_obj_invalidate(obj);
}

void lvppCanvasIndexed::drawRectWithFill(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColor, lv_color_t fillColor) {
    lv_color_t bColInd, fColInd;
    if (getIndexFromColor(borderColor, bColInd) && getIndexFromColor(fillColor, fColInd)) {
        drawRectWithFillByIndex(x1, y1, w, h, bColInd, fColInd);
    }
    else {
        LV_LOG_WARN("drawRectWithFill: border or fill color not found in palette.");
    }
}

void lvppCanvasIndexed::drawRectWithFillByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColorInd, lv_color_t fillColorInd) {
    if (borderColorInd.full>=colorIndexesUsed) {
        LV_LOG_WARN("drawRectWithFillByIndex: border index out of palette range.");
        return;
    }
    if (fillColorInd.full>=colorIndexesUsed) {
        LV_LOG_WARN("drawRectWithFillByIndex: fill index out of palette range.");
        return;
    }

    for (lv_coord_t x=x1 ; x<x1+w; x++)
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x, y1, borderColorInd);

    for (lv_coord_t y=y1; y<y1+h; y++)
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x1, y, borderColorInd);

    for (lv_coord_t y=y1; y<y1+h; y++)
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x1+w, y, borderColorInd);

    for (lv_coord_t x=x1 ; x<x1+w; x++)
        lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x, y1+h, borderColorInd);

    // Now for the fill operation
    for (lv_coord_t y=y1+1; y<y1+h; y++) {
        for (lv_coord_t x=x1+1 ; x<x1+w; x++) {
            lv_img_buf_set_px_color(&((lv_canvas_t*)obj)->dsc, x, y, fillColorInd);
        }
    }
    
    lv_obj_invalidate(obj);
}

void lvppCanvasIndexed::drawCenteredRectWithoutFill(lv_coord_t xBorder, lv_coord_t yBorder, lv_color_t borderColor) {
    lv_color_t bColInd;
    if (getIndexFromColor(borderColor, bColInd)) {
        drawCenteredRectWithoutFillByIndex(xBorder, yBorder, bColInd);
    }
    else {
        LV_LOG_WARN("drawCenteredRectWithoutFill: border color not found in palette.");
    }
}

void lvppCanvasIndexed::drawCenteredRectWithoutFillByIndex(lv_coord_t xBorder, lv_coord_t yBorder, lv_color_t borderColorInd) {
    lv_coord_t x,y,w,h;
    // Calculate xywh based on width/height of canvas and border amount.
    if (xBorder >= width/2 || yBorder >= height/2) {
        LV_LOG_WARN("drawCenteredRectWithoutFillByIndex: xBorder or yBorder exceed half of width/height. NOT Drawn.");
        return;
    }

    x = xBorder;
    y = yBorder;
    w = width - xBorder*2;
    h = height - yBorder*2;
    drawRectWithoutFillByIndex(x, y, w, h, borderColorInd);
}

void lvppCanvasIndexed::drawCenteredRectWithFill(lv_coord_t xBorder, lv_coord_t yBorder, 
        lv_color_t borderColor, lv_color_t fillColor) {
    lv_color_t bColInd, fColInd;
    if (getIndexFromColor(borderColor, bColInd) && getIndexFromColor(fillColor, fColInd)) {
        drawCenteredRectWithFillByIndex(xBorder, yBorder, bColInd, fColInd);
    }
    else {
        LV_LOG_WARN("drawCenteredRectWithFill: border or fill color not found in palette.");
    }
}

void lvppCanvasIndexed::drawCenteredRectWithFillByIndex(lv_coord_t xBorder, lv_coord_t yBorder, 
        lv_color_t borderColorInd, lv_color_t fillColorInd) {
    lv_coord_t x,y,w,h;
    // Calculate xywh based on width/height of canvas and border amount.
    if (xBorder >= width/2 || yBorder >= height/2) {
        LV_LOG_WARN("drawCenteredRectWithFillByIndex: xBorder or yBorder exceed half of width/height. NOT Drawn.");
        return;
    }

    x = xBorder;
    y = yBorder;
    w = width - xBorder*2;
    h = height - yBorder*2;
    drawRectWithFillByIndex(x, y, w, h, borderColorInd, fillColorInd);
}

lvppCanvasFullColor::lvppCanvasFullColor(const char* fName, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_color_t* providedBuffer, lv_obj_t* parent) 
    : lvppBase(fName, "CANVASFULLCOLOR") {

    width = w;
    height = h;
    
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_canvas_create(objParent));

    pBuffer = (lv_color_t*)malloc(sizeof(lv_color_t)*LV_CANVAS_BUF_SIZE_TRUE_COLOR(w, h));
    assert(pBuffer);

    lv_canvas_set_buffer(obj, pBuffer, w, h, LV_IMG_CF_TRUE_COLOR);
    align(LV_ALIGN_TOP_LEFT, x, y);

    pDscLabel = nullptr;
    pDscLine  = nullptr;
    pDscRect  = nullptr;
}

lvppCanvasFullColor::~lvppCanvasFullColor() {
    if (pDscLabel) {
        delete pDscLabel;
    }
    if (pDscLine) {
        delete pDscLine;
    }
    if (pDscRect) {
        delete pDscRect;
    }
    if (pBuffer) {
        free(pBuffer);
    }
}

void lvppCanvasFullColor::setbgColor(lv_color_t bgColor) {
    lv_canvas_fill_bg(obj, bgColor, LV_OPA_COVER);
}

void lvppCanvasFullColor::drawPixel(lv_coord_t x, lv_coord_t y, lv_color_t color) {
    lv_canvas_set_px_color(obj, x, y, color);
}

void lvppCanvasFullColor::drawRect(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, 
        lv_coord_t borderThickness, lv_color_t borderColor, lv_color_t fillColor,  uint16_t radius, lv_opa_t opa) {

    if (!pDscRect) {
        pDscRect = new lv_draw_rect_dsc_t;
        lv_draw_rect_dsc_init(pDscRect);
    }

    pDscRect->radius = radius;
    pDscRect->bg_opa = opa;
    pDscRect->bg_color = fillColor;
//    rect_dsc.bg_grad.dir = LV_GRAD_DIR_HOR;
//    rect_dsc.bg_grad.stops[0].color = lv_palette_main(LV_PALETTE_RED);
//    rect_dsc.bg_grad.stops[1].color = lv_palette_main(LV_PALETTE_BLUE);
    pDscRect->border_width = borderThickness;
    pDscRect->border_opa = LV_OPA_100;
    pDscRect->border_color = borderColor;
    lv_canvas_draw_rect(obj, x, y, w, h, pDscRect);
}

void lvppCanvasFullColor::drawLine(lv_coord_t x1, lv_coord_t y1, lv_coord_t x2, lv_coord_t y2, lv_coord_t width, lv_color_t color) {

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

void lvppCanvasFullColor::drawLabel(lv_coord_t x, lv_coord_t y, lv_coord_t maxW, lv_color_t color, const char* pText) {

    if (!pText)
        return;

    if (!pDscLabel) {
        pDscLabel = new lv_draw_label_dsc_t;
        lv_draw_label_dsc_init(pDscLabel);
    }

    pDscLabel->color = color;

    lv_canvas_draw_text(obj, x, y, maxW, pDscLabel, pText);
}
