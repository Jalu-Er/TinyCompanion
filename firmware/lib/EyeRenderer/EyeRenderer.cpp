/**
 * @file EyeRenderer.cpp
 * @brief Implementation of OLED eye graphics rendering calculations.
 */

#include "EyeRenderer.h"

EyeRenderer::EyeRenderer(IOledDisplay& oled)
    : display(oled) {}

EyeGeometry EyeRenderer::calculateGeometry(const Expression& expr) {
    EyeGeometry geo;
    
    // Common properties
    uint8_t w = EyeConfig::DEFAULT_WIDTH;
    uint8_t h = EyeConfig::DEFAULT_HEIGHT;
    uint8_t r = EyeConfig::CORNER_RADIUS;
    uint8_t pupilR = expr.pupilRadius;
    uint8_t closedH = 0;
    
    if (expr.eyelidOpen < 100) {
        closedH = (static_cast<uint16_t>(h) * (100 - expr.eyelidOpen)) / 100;
    }
    
    // Left eye calculations
    geo.left.cx = EyeConfig::LEFT_CENTER_X;
    geo.left.cy = EyeConfig::CENTER_Y;
    geo.left.w = w;
    geo.left.h = h;
    geo.left.r = r;
    geo.left.pupilR = pupilR;
    geo.left.closedH = closedH;
    geo.left.pupilCx = geo.left.cx;
    geo.left.pupilCy = geo.left.cy;
    geo.left.x = geo.left.cx - w / 2;
    geo.left.y = geo.left.cy - h / 2;
    
    // Right eye calculations
    geo.right.cx = EyeConfig::RIGHT_CENTER_X;
    geo.right.cy = EyeConfig::CENTER_Y;
    geo.right.w = w;
    geo.right.h = h;
    geo.right.r = r;
    geo.right.pupilR = pupilR;
    geo.right.closedH = closedH;
    geo.right.pupilCx = geo.right.cx;
    geo.right.pupilCy = geo.right.cy;
    geo.right.x = geo.right.cx - w / 2;
    geo.right.y = geo.right.cy - h / 2;
    
    // Adjustments based on shape
    switch (expr.eyeShape) {
        case EyeShape::HAPPY:
            geo.left.pupilCy = geo.left.cy + EyeConfig::HAPPY_Y_OFFSET;
            geo.left.pupilR = pupilR + EyeConfig::HAPPY_PUPIL_ADD;
            geo.right.pupilCy = geo.right.cy + EyeConfig::HAPPY_Y_OFFSET;
            geo.right.pupilR = pupilR + EyeConfig::HAPPY_PUPIL_ADD;
            break;
            
        case EyeShape::SAD:
            geo.left.pupilCy = geo.left.cy + EyeConfig::SAD_Y_OFFSET;
            geo.right.pupilCy = geo.right.cy + EyeConfig::SAD_Y_OFFSET;
            break;
            
        case EyeShape::ANGRY:
            geo.left.pupilCy = geo.left.cy + EyeConfig::ANGRY_Y_OFFSET;
            geo.right.pupilCy = geo.right.cy + EyeConfig::ANGRY_Y_OFFSET;
            break;
            
        case EyeShape::ALERT:
            break;
            
        case EyeShape::SLEEPY:
            break;
            
        case EyeShape::SQUINT:
            geo.left.h = (static_cast<uint16_t>(h) * 2) / 3;
            geo.left.y = geo.left.cy - geo.left.h / 2; // Keep centered around center_y
            geo.left.pupilR = (pupilR > 1) ? (pupilR - 1) : 0;
            
            geo.right.h = (static_cast<uint16_t>(h) * 2) / 3;
            geo.right.y = geo.right.cy - geo.right.h / 2; // Keep centered around center_y
            geo.right.pupilR = (pupilR > 1) ? (pupilR - 1) : 0;
            break;
            
        default:
            break;
    }
    
    return geo;
}

void EyeRenderer::drawEye(const SingleEyeGeometry& eyeGeo, EyeShape shape) {
    switch (shape) {
        case EyeShape::NORMAL:
            // 1. Draw solid rounded eye structure
            display.fillRoundRect(eyeGeo.x, eyeGeo.y, eyeGeo.w, eyeGeo.h, eyeGeo.r, 1);
            
            // 2. Draw black pupil inside
            display.fillCircle(eyeGeo.pupilCx, eyeGeo.pupilCy, eyeGeo.pupilR, 0);
            
            // 3. Eyelid top clipping closure
            if (eyeGeo.closedH > 0) {
                display.fillRect(eyeGeo.x - 2, eyeGeo.y - 2, eyeGeo.w + 4, eyeGeo.closedH + 2, 0);
            }
            break;
            
        case EyeShape::HAPPY:
            // Smile shape: filled rounded rect cut off at bottom and center
            display.fillRoundRect(eyeGeo.x, eyeGeo.y, eyeGeo.w, eyeGeo.h, eyeGeo.r, 1);
            
            // Overwrite lower half to construct flat bottom smiling curve
            display.fillRect(eyeGeo.x - 2, eyeGeo.cy + 2, eyeGeo.w + 4, eyeGeo.h / 2 + 4, 0);
            
            // Inner carve out to form hollow curved line
            display.fillCircle(eyeGeo.pupilCx, eyeGeo.pupilCy, eyeGeo.pupilR, 0);
            break;
        
        case EyeShape::SAD:
            // Drooped eyelids: filled round rect with aggressive top-half coverage
            display.fillRoundRect(eyeGeo.x, eyeGeo.y, eyeGeo.w, eyeGeo.h, eyeGeo.r, 1);
            display.fillCircle(eyeGeo.pupilCx, eyeGeo.pupilCy, eyeGeo.pupilR, 0);
            
            // Diagonal top cuts representing drooped heavy sad brows
            display.fillRect(eyeGeo.x - 2, eyeGeo.y - 2, eyeGeo.w + 4, eyeGeo.h / 3, 0);
            break;
        
        case EyeShape::ANGRY:
            // Angry: filled rounded rect with top slant cuts
            display.fillRoundRect(eyeGeo.x, eyeGeo.y, eyeGeo.w, eyeGeo.h, eyeGeo.r, 1);
            display.fillCircle(eyeGeo.pupilCx, eyeGeo.pupilCy, eyeGeo.pupilR, 0);
            
            // Angular brow cover
            display.fillRect(eyeGeo.x - 2, eyeGeo.y - 2, eyeGeo.w + 4, eyeGeo.h / 3 + 2, 0);
            break;
        
        case EyeShape::ALERT:
            // Wide open round eye
            display.fillCircle(eyeGeo.cx, eyeGeo.cy, eyeGeo.w / 2, 1);
            display.fillCircle(eyeGeo.pupilCx, eyeGeo.pupilCy, eyeGeo.pupilR, 0);
            break;
            
        case EyeShape::SLEEPY:
            // Sleepy/Closed: Simple horizontal line represent closed lashes
            display.fillRect(eyeGeo.x, eyeGeo.cy - 1, eyeGeo.w, 3, 1);
            break;
            
        case EyeShape::SQUINT:
            // Concentrated look / Thinking
            display.fillRoundRect(eyeGeo.x, eyeGeo.y, eyeGeo.w, eyeGeo.h, eyeGeo.r, 1);
            display.fillCircle(eyeGeo.pupilCx, eyeGeo.pupilCy, eyeGeo.pupilR, 0);
            break;
            
        default:
            // Cross shape representing Error state
            display.drawLine(eyeGeo.cx - EyeConfig::ERROR_CROSS_SIZE, eyeGeo.cy - EyeConfig::ERROR_CROSS_SIZE,
                             eyeGeo.cx + EyeConfig::ERROR_CROSS_SIZE, eyeGeo.cy + EyeConfig::ERROR_CROSS_SIZE, 1);
            display.drawLine(eyeGeo.cx - EyeConfig::ERROR_CROSS_SIZE, eyeGeo.cy + EyeConfig::ERROR_CROSS_SIZE,
                             eyeGeo.cx + EyeConfig::ERROR_CROSS_SIZE, eyeGeo.cy - EyeConfig::ERROR_CROSS_SIZE, 1);
            break;
    }
}

void EyeRenderer::render(const Expression& expr) {
    // 1. Clear previous screen frame buffer contents
    display.clear();
    
    // 2. Compute all coordinate parameters once
    EyeGeometry geo = calculateGeometry(expr);
    
    // 3. Draw Left Eye
    drawEye(geo.left, expr.eyeShape);
    
    // 4. Draw Right Eye
    drawEye(geo.right, expr.eyeShape);
}
