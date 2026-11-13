#include "gamestates/draw.h"
#include "balloonist.h"
#include "buffers.h"
#include "camera.h"
#include "common.h"
#include "cutscene.h"
#include "cyclorama.h"
#include "dragon.h"
#include "environment.h"
#include "fairy.h"
#include "game_over.h"
#include "gamepad.h"
#include "graphics.h"
#include "hud.h"
#include "images.h"
#include "loaders.h"
#include "math.h"
#include "memory.h"
#include "moby.h"
#include "moby_lists.h"
#include "overlay_pointers.h"
#include "renderers.h"
#include "sony_image.h"
#include "specular_and_metal.h"
#include "spyro.h"
#include "strings.h"
#include "titlescreen.h"
#include "variables.h"

#include <libetc.h>
#include <libgpu.h>
#include <stdio.h>

extern unsigned int strlen(char *);

#if GAME_VER >= VER_C3_DEMO_NTSC_U
void ClearFramebuffers(void) {
  RECT rc;
  setRECT(&rc, 0, 0, 512, 240);
  ClearImage(&rc, 0, 0, 0);
  rc.y = 240;
  ClearImage(&rc, 0, 0, 0);
  DrawSync(0);
}

void FadeStartupImage(void *src, u_long *imageBuffer, int bfadeIn) {
  RECT rc;
  int i;
  const int blend_start = 224;
  const int blend_step = 32;
  const int blend_step_count = 8;
  const int rgb24_w = (512 / 2) * 3;
  const int rgb24_h = 240;
  const int image_size = 512 * 3 * 240;

  Memset(imageBuffer, 0, image_size);

  for (i = 0; i < blend_step_count; i++) {
    if (bfadeIn) {
      func_80017F24(src, imageBuffer, -blend_start + i * blend_step);
    } else {
      func_80017F24(src, imageBuffer, -i * blend_step - blend_step);
    }

    setRECT(&rc, 0, 0, rgb24_w, rgb24_h);

    LoadImage(&rc, imageBuffer);
    DrawSync(0);
    VSync(0);
    PutDispEnv(&g_DB[1].m_DispEnv);

    if (bfadeIn && i == 0) {
      SetDispMask(1);
    }
  }
}
#endif

// HAS to be local to this file.
D_80075950_t D_80075950;

extern char D_8006F304[1];             // "GAMEOVER"
extern u_short g_GrayscalePalette[32]; // 32 colors

/// @brief Create text Mobys, no capitalization
#if GAME_VER == VER_NTSC_U
Moby *func_80017FE4(char *pText, Vector3D *pPosition, int pSpaceWidth,
                    int pShadeIndex) {
  while (*pText != '\0') {
    if (*pText != ' ') {
      g_HudMobys -= 1;
      Memset(g_HudMobys, 0, sizeof(Moby));
      VecCopy(&g_HudMobys->m_Position, pPosition);

      if (*pText >= '0' && *pText <= '9') {
        g_HudMobys->m_Class = MOBYCLASS_NUMBER_0 + *pText - '0';
      } else if (*pText >= 'A' && *pText <= 'Z') {
        g_HudMobys->m_Class = MOBYCLASS_LETTER_A + *pText - 'A';
      } else if (*pText == '/') {
        g_HudMobys->m_Class = MOBYCLASS_SLASH;
      } else if (*pText == '?') {
        g_HudMobys->m_Class = MOBYCLASS_QUESTION_MARK;
      } else if (*pText == '%') {
        g_HudMobys->m_Class = MOBYCLASS_PERCENT;
      } else if (*pText == '^') {
        g_HudMobys->m_Class = MOBYCLASS_CARET;
      } else if (*pText == '+') {
        g_HudMobys->m_Class = MOBYCLASS_PLUS;
      } else {
        g_HudMobys->m_Class = MOBYCLASS_PERIOD;
      }
      g_HudMobys->m_DepthOffset = 0x7F;
      g_HudMobys->m_SpecularMetalType = pShadeIndex;
      g_HudMobys->m_RenderRadius = 0xFF;
    }
    pText++;
    pPosition->x += pSpaceWidth;
  }
  return g_HudMobys;
}
#endif

/// @brief Create text Mobys
#if GAME_VER == VER_NTSC_U
Moby *func_800181AC(char *pText, Vector3D *pPosition, Vector3D *pTextSpacing,
                    int pSpaceWidth, int pShadeIndex) {
  int isCapital;
  int spaceSize;

  isCapital = 1;
  while (*pText != '\0') {
    if (*pText != ' ') {
      g_HudMobys -= 1;
      Memset(g_HudMobys, 0, sizeof(Moby));
      VecCopy(&g_HudMobys->m_Position, pPosition);
      if (*pText == '!' || *pText == '?') {
        isCapital = 1;
      }
      if (!isCapital) {
        g_HudMobys->m_Position.y += pTextSpacing->y;
        g_HudMobys->m_Position.z = pTextSpacing->z;
      }
      if (*pText >= '0' && *pText <= '9') {
        g_HudMobys->m_Class = MOBYCLASS_NUMBER_0 + *pText - '0';
      } else if (*pText >= 'A' && *pText <= 'Z') {
        g_HudMobys->m_Class = MOBYCLASS_LETTER_A + *pText - 'A';
      } else if (*pText == '!') {
        g_HudMobys->m_Class = MOBYCLASS_EXCLAMATION_MARK;
      } else if (*pText == ',') {
        g_HudMobys->m_Class = MOBYCLASS_LETTER_APOSTROPHE;
      } else if (*pText == '?') {
        g_HudMobys->m_Class = MOBYCLASS_QUESTION_MARK;
      } else if (*pText == '.') {
        g_HudMobys->m_Class = MOBYCLASS_PERIOD;
      } else {
        g_HudMobys->m_Class = MOBYCLASS_LETTER_APOSTROPHE;
        g_HudMobys->m_Position.y -= pTextSpacing->x * 2 / 3;
      }
      g_HudMobys->m_DepthOffset = 0x7F;
      g_HudMobys->m_SpecularMetalType = pShadeIndex;
      g_HudMobys->m_RenderRadius = 0xFF;
      if (isCapital) {
        pPosition->x += pSpaceWidth;
      } else {
        pPosition->x += pTextSpacing->x;
      }
      if (*pText >= '0' && *pText <= '9') {
        isCapital = 1;
      } else {
        isCapital = 0;
      }
    } else {
      spaceSize = pTextSpacing->x * 3;
      isCapital = 1;
      if (spaceSize < 0) {
        spaceSize += 3;
      }
      pPosition->x += spaceSize >> 2;
    }
    pText++;
  }
  return g_HudMobys;
}
#else
// Used in C3 NTSC-U Demo
Moby *func_800181AC(char *pText, Vector3D *pPosition, Vector3D *pTextSpacing,
                    int pSpaceWidth, int pShadeIndex) {
  int isCapital;
  int wasCapital;

  isCapital = 1;
  wasCapital = 1;

  while (*pText != '\0') {
    if (*pText != ' ') {
      g_HudMobys -= 1;
      Memset(g_HudMobys, 0, sizeof(Moby));
      VecCopy(&g_HudMobys->m_Position, pPosition);

      if (*pText >= 'A' && *pText <= 'Z') {
        g_HudMobys->m_Class = MOBYCLASS_LETTER_A + *pText - 'A';
      } else if (*pText >= '0' && *pText <= '9') {
        isCapital = 1;
        g_HudMobys->m_Class = MOBYCLASS_NUMBER_0 + *pText - '0';
      } else if (*pText >= 'a' && *pText <= 'f') {
        if (wasCapital) {
          pPosition->x -= pSpaceWidth;
        } else {
          pPosition->x -= pTextSpacing->x;
        }

        g_HudMobys->m_Position.x = pPosition->x;

        if (*pText >= 'a' && *pText <= 'e') {
          g_HudMobys->m_Class = *pText + 0x10C;
        }

        if (*pText == 'f') {
          g_HudMobys->m_Class = 0x147;
          g_HudMobys->m_Position.y -= pSpaceWidth / 3;
        }
        isCapital = wasCapital;

      } else if (*pText == '/') {
        isCapital = 1;
        g_HudMobys->m_Class = 0x115;
      } else if (*pText == '-') {
        g_HudMobys->m_Class = 0x115;
        g_HudMobys->m_Rotation.x = 0x29;
        g_HudMobys->m_Position.z += 0xA00;
        isCapital = 1;
      } else if (*pText == '%') {
        isCapital = 1;
        g_HudMobys->m_Class = 0x110;
      } else if (*pText == '^') {
        isCapital = 1;
        g_HudMobys->m_Class = 0x141;
      } else if (*pText == '+') {
        isCapital = 1;
        g_HudMobys->m_Class = 0x13D;
      } else if (*pText == '!') {
        isCapital = 1;
        g_HudMobys->m_Class = MOBYCLASS_EXCLAMATION_MARK;
      } else if (*pText == '?') {
        isCapital = 1;
        g_HudMobys->m_Class = MOBYCLASS_QUESTION_MARK;
      } else if (*pText == '.') {
        g_HudMobys->m_Class = MOBYCLASS_PERIOD;
      } else if (*pText == ',') {
        g_HudMobys->m_Class = MOBYCLASS_LETTER_APOSTROPHE;
      } else {
        g_HudMobys->m_Class = MOBYCLASS_LETTER_APOSTROPHE;
        g_HudMobys->m_Position.y -= (pSpaceWidth * 2) / 3;
      }

      g_HudMobys->m_DepthOffset = 0x7F;
      g_HudMobys->m_SpecularMetalType = pShadeIndex;
      g_HudMobys->m_RenderRadius = 0xFF;

      if (isCapital) {
        pPosition->x += pSpaceWidth;
      } else {
        pPosition->x += pTextSpacing->x;
        g_HudMobys->m_Position.y += pTextSpacing->y;
        g_HudMobys->m_Position.z = pTextSpacing->z;
      }

      wasCapital = isCapital;

      if (((*pText >= 'A' && *pText <= 'Z') ||
          (*pText >= 'a' && *pText <= 'f')) && pTextSpacing) {
        isCapital = 0;
      } else {
        isCapital = 1;
      }
    } else {
      pPosition->x += (pSpaceWidth * 2) / 3;
      isCapital = 1;
    }

    pText++;
  }

  return g_HudMobys;
}
#endif

/// @brief Creates shaded line
void func_8001844C(int pX0, int pY0, int pX1, int pY1) {
  u_char intensity;
  LINE_G2 *line;

  line = D_800757B0;
  line->tag = PRIM_TAG(LINE_G2);
  setcode(line, PRIM_LINE_G2);
  setXY2(line, pX0, pY0, pX1, pY1);
  intensity = 128 - (u_char)func_80017908(Atan2Fast(pX0 - 256, pY0 - 120),
                                          g_RenderEffects.m_SpecularTime);
  setRGB0(line, intensity + 96, intensity + 96, intensity);
  intensity = 128 - (u_char)func_80017908(Atan2Fast(pX1 - 256, pY1 - 120),
                                          g_RenderEffects.m_SpecularTime);
  setRGB1(line, intensity + 96, intensity + 96, intensity);
  // Link Prim
  func_800168DC(line);
  D_800757B0 = line + 1;
}

/// @brief Creates an arrow
void func_80018534(Vector3D *pPosition, int pTimer, int pRotate) {
  if ((pTimer & 31) < 16) {
    g_HudMobys--;
    Memset(g_HudMobys, 0, sizeof(Moby));
    g_HudMobys->m_Class = MOBYCLASS_NUMBER_1;
    VecCopy(&g_HudMobys->m_Position, pPosition);
    if (pRotate < 2) {
      g_HudMobys->m_Rotation.x = 64;
      g_HudMobys->m_Rotation.z = pRotate * 128;
    }
    g_HudMobys->m_DepthOffset = 127;
    g_HudMobys->m_SpecularMetalType = 11;
    g_HudMobys->m_RenderRadius = 0xFF;
  }
}

/// @brief Creates a shaded box
void func_8001860C(int pX0, int pX1, int pY0, int pY1) {
  DR_MODE *drm;
  POLY_F4 *f4;
  drm = D_800757B0;
  SetDrawMode(drm, 1, 0, getTPage(0, 2, 0, 0), 0);
  func_800168DC(drm);
  f4 = (POLY_F4 *)(drm + 1);
  f4->tag = PRIM_TAG(POLY_F4);
  setcode(f4, PRIM_STP(PRIM_POLY_F4));
  f4->x0 = pX0;
  f4->x1 = pX1;
  f4->x2 = pX0;
  f4->x3 = pX1;
  f4->y0 = pY0;
  f4->y1 = pY0;
  f4->y2 = pY1;
  f4->y3 = pY1;
  setRGB0(f4, 112, 112, 112);
  func_800168DC(f4);
  D_800757B0 = f4 + 1;
  func_8001844C(pX0, pY0, pX1, pY0);
  func_8001844C(pX1, pY0, pX1, pY1);
  func_8001844C(pX1, pY1, pX0, pY1);
  func_8001844C(pX0, pY1, pX0, pY0);
}

/// @brief Creates rescued dragon text
void func_80018728(void) {
#if GAME_VER == VER_NTSC_U
  char *name;
  int length;
  Moby *mobys;
  Vector3D pos;
  int i;

  // TODO: Dragon Moby props struct
  name =
      g_DragonNames[((int *)g_DragonCutscene.m_RescuedDragonMoby->m_Props)[14]];
  length = strlen(name) - 1;
  mobys = g_HudMobys;
  pos.x = 176 - length * 13;
  pos.y = 200;
  pos.z = 4096;
  func_80017FE4("RESCUED", &pos, 20, 2);
  pos.x = 336 - length * 13;
  pos.y = 200;
  pos.z = 3072;
  func_80017FE4(name, &pos, 26, 2);
#else
  char buf[32];
  Vector3D pos;
  Vector3D spacing;
  Moby *mobys;
  int length;
  int i;

  // TODO: Dragon Moby props struct
  sprintf(buf, "%s %s", StringRescued,
          g_DragonNames[((int *)g_DragonCutscene.m_RescuedDragonMoby->m_Props)[14]]);

  length = strlen(buf) - 1;
  mobys = g_HudMobys;

  pos.x = 254 - length * 8;
  pos.y = 200;
  pos.z = 0x1100;

  spacing.x = 0x10;
  spacing.y = 1;
  spacing.z = 0x1400;

  func_800181AC(buf, &pos, &spacing, 18, 11);
#endif

  mobys--;
  for (i = 0; (int)g_HudMobys <= (int)mobys; i++) {
    mobys->m_Rotation.z =
        COSINE_8((g_DragonCutscene.unk_0x50 * 2 + i * 12) & 0xFF) >> 7;
    mobys--;
  }
}

// Unused, boy I wonder.. probably the old dragon cutscenes
void func_80018878(void) {}

/// @brief Copies the HUD mobys to the shaded mobys
void func_80018880(void) {
  Moby **currentMoby = g_SonyImage.m_ShadedMobys;

  // Find the first empty slot in the array
  while (*currentMoby) {
    currentMoby++;
  }

  // Append remaining HUD mobys to the shaded moby array
  while (g_HudMobys != D_800756FC) {
    *currentMoby = g_HudMobys;
    g_HudMobys++;
    currentMoby++;
  }

  // Null-terminate the array
  *currentMoby = 0;
}

/// @brief Creates the demo mode text
void func_80018908(void) {
  Vector3D position;
  Vector3D spacing;
  Moby *mobys;
  int i;

  mobys = g_HudMobys;
  spacing.x = 16;
  spacing.y = 1;
  spacing.z = 5120;
#if GAME_VER == VER_NTSC_U
  position.x = 199;
#else
  position.x = StringDemoModeX;
#endif
  position.y = 200;
  position.z = 4352;
#if GAME_VER == VER_NTSC_U
  func_800181AC("DEMO MODE", &position, &spacing, 18, 2);
#else
  func_800181AC(StringDemoMode, &position, &spacing,
                18, 11);
#endif
  mobys--;
  for (i = 0; (int)g_HudMobys <= (int)mobys; i++) {
    mobys->m_Rotation.z = COSINE_8((g_LevelTicks * 4 + i * 12) & 0xFF) >> 7;
    mobys--;
  }

  func_80018880();
}

/// @brief Creates tracers, C function, god knows why
#if GAME_VER == VER_NTSC_U
void func_800189F0(void) {

  int i, j;
  int temp;

  int x0, x1, y0, y1, x2, x3, y2, y3;
  int r1, g1, b1, r2, g2, b2;

  int point_count;
  TracerPoint *tracer_ptr;
  Vector3D delta;

  int width;
  int magnitude;

  uint ot_z;
  int intensity;
  int prev_intensity;

  point_count = 0;

  for (i = 0; i < g_TracerCount; i++) {
    tracer_ptr = g_TracerLists[i];

    // world to screen
    for (j = 0; j < g_TracerPointCount[i]; j++) {
      func_80017B48(&tracer_ptr[j].ScreenPos, &tracer_ptr[j].WorldPos, 1);
    }

    // build tracer prims
    for (j = 0; j < g_TracerPointCount[i] - 1; j++) {

      width = 70000;
      if (j == g_TracerPointCount[i] - 2) {
        width = 0;
      }
      if (j < 3) {
        width = j * 20000;
      }
      if (g_LevelId == 63) {
        width = width >> 2;
      }

      if (tracer_ptr[j].Age <= 80) {
        point_count++;

        // delta = current - next
        VecSub(&delta, &tracer_ptr[j].ScreenPos, &tracer_ptr[j + 1].ScreenPos);

        // rotate 90 degrees
        temp = -delta.x;
        delta.x = delta.y;
        delta.y = temp;

        magnitude = delta.x * delta.x + delta.y * delta.y;
        magnitude = SquareRoot(magnitude);

        if (magnitude == 0 || tracer_ptr[j].ScreenPos.z == 0) {
          delta.x = 0;
          delta.y = 0;
        } else {
          delta.x = (delta.x * width) / (tracer_ptr[j].ScreenPos.z * magnitude);
          delta.y = (delta.y * width) / (tracer_ptr[j].ScreenPos.z * magnitude);
        }

        // quad coords
        x0 = tracer_ptr[j].ScreenPos.x + delta.x;
        x1 = tracer_ptr[j].ScreenPos.x - delta.x;
        y0 = tracer_ptr[j].ScreenPos.y + delta.y;
        y1 = tracer_ptr[j].ScreenPos.y - delta.y;

        if (point_count > 1) {
          ot_z = tracer_ptr[j].ScreenPos.z >> 7;
          if (ot_z < 2000) {
            DR_MODE *drm;
            POLY_G4 *g4;

            intensity = tracer_ptr[j].Age;
            prev_intensity = tracer_ptr[j - 1].Age;

            r1 = 150 - 2 * intensity;
            g1 = 150 - 4 * intensity;
            b1 = 150 - 10 * intensity;

            r2 = 150 - 2 * prev_intensity;
            g2 = 150 - 4 * prev_intensity;
            b2 = 150 - 10 * prev_intensity;

            // clamp
            if (r1 < 0)
              r1 = 0;
            if (g1 < 0)
              g1 = 0;
            if (b1 < 0)
              b1 = 0;
            if (r2 < 0)
              r2 = 0;
            if (g2 < 0)
              g2 = 0;
            if (b2 < 0)
              b2 = 0;

            if (g_LevelId == 63) {
              temp = r1;
              r1 = g1;
              g1 = temp;
              temp = r2;
              r2 = g2;
              g2 = temp;
            }
            drm = D_800757B0;
            SetDrawMode(drm, 1, 0, getTPage(0, 1, 0, 0), 0);
            func_800168A0(drm, ot_z);
            g4 = (POLY_G4 *)(drm + 1);
            g4->tag = PRIM_TAG(POLY_G4);
            setcode(g4, PRIM_STP(PRIM_POLY_G4));
            g4->x0 = x0;
            g4->x1 = x1;
            g4->x2 = x2;
            g4->x3 = x3;
            g4->y0 = y0;
            g4->y1 = y1;
            g4->y2 = y2;
            g4->y3 = y3;
            setRGB0(g4, r1, g1, b1);
            setRGB1(g4, r1, g1, b1);
            setRGB2(g4, r2, g2, b2);
            setRGB3(g4, r2, g2, b2);

            func_800168A0(g4, ot_z);
            D_800757B0 = g4 + 1;
          }
        }
        x2 = x0;
        x3 = x1;
        y2 = y0;
        y3 = y1;
      }
    }
  }
  g_TracerCount = 0;
}
#endif

/// @brief Creates the border
void func_80018F30(void) {
  POLY_F4 *bar;

  if (g_ScreenBorderEnabled != 0) {
    if (D_800756C0 <= 21) {
      D_800756C0 += g_DeltaTime;
    }
    if (D_800756C0 >= 23) {
      D_800756C0 = 22;
    }
  } else {
    if (D_800756C0 > 0) {
      D_800756C0 -= g_DeltaTime;
    }
    if (D_800756C0 < 0) {
      D_800756C0 = 0;
    }
  }
  bar = D_800757B0;
  bar->tag = PRIM_TAG(POLY_F4);
  bar->code = PRIM_POLY_F4;
  bar->x0 = 0;
  bar->x1 = 512;
  bar->x2 = 0;
  bar->x3 = 512;
  bar->y0 = 0;
  bar->y1 = 0;
  bar->y2 = D_800756C0;
  bar->y3 = D_800756C0;
  setRGB0(bar, 0, 0, 0);
  func_800168DC(bar);
  D_800757B0 = ++bar;
  bar->tag = PRIM_TAG(POLY_F4);
  bar->code = PRIM_POLY_F4;
  bar->y2 = 240;
  bar->y3 = 240;
  bar->x0 = 0;
  bar->x1 = 512;
  bar->x2 = 0;
  bar->x3 = 512;
  bar->y0 = 240 - D_800756C0;
  bar->y1 = 240 - D_800756C0;
  setRGB0(bar, 0, 0, 0);
  func_800168DC(bar);
  D_800757B0 = bar + 1;
}

/// @brief Creates fade in/out
// Not u_char, because the input is not AND'd
void func_800190D4(int pMode, int pR, int pG, int pB) {
  DR_MODE *drm;
  POLY_F4 *f4;

  drm = D_800757B0;
  f4 = (POLY_F4 *)(drm + 1);
  SetDrawMode(drm, 1, 0, pMode << 5, 0);
  func_800168DC(drm);

  f4->tag = PRIM_TAG(POLY_F4);

  setcode(f4, PRIM_STP(PRIM_POLY_F4));
  setXYWH(f4, 0, 8, 512, 240 - 16);
  setRGB0(f4, pR, pG, pB);

  func_800168DC(f4);

  D_800757B0 = f4 + 1;
}

/// @brief Creates a 2D sprite
void func_8001919C(RECT *pDest, Tiledef *pUV, ColorInt *pColor) {

  POLY_FT4 *ft4;
  ft4 = D_800757B0;
  ft4->tag = PRIM_TAG(POLY_FT4);
  setcode(ft4, PRIM_POLY_FT4);

  if (pColor == 0)
    setRGB0(ft4, 0x80, 0x80, 0x80);
  else
    setRGB0(ft4, pColor->r, pColor->g, pColor->b);

  setXYWH(ft4, pDest->x, pDest->y, pDest->w, pDest->h);

  // ft4 is being read as ints
  *(int *)&ft4->u0 = pUV->uv0.all;
  *(int *)&ft4->u1 = pUV->uv1.all;

  ft4->u1 = ft4->u0 + pDest->w;
  ft4->u2 = ft4->u0;
  ft4->u3 = ft4->u1;
  ft4->v2 = ft4->v0 + pDest->h;
  ft4->v3 = ft4->v0 + pDest->h;
  func_800168DC(ft4);
  D_800757B0 = ft4 + 1;
}

/// @brief Creates the collectable icons and counters
void func_80019300(void) {
  int i;
  char *text;
  Moby *mobys;
  Moby **shaded_mobys;

  if (g_IsFlightLevel == 0) {
    shaded_mobys = g_SonyImage.m_ShadedMobys;

    while (*shaded_mobys != nullptr)
      shaded_mobys++;

    if (g_Hud.m_GemDisplayState != 0) {
      for (i = g_Hud.m_GemDisplayState == 4 ? 4 : 0; i < 5; i++) {
        *shaded_mobys++ = &g_Hud.m_Mobys[i];
      }

      if (g_Hud.m_GemDisplayState == 4) {
        char text[16];
        Vector3D vec;
        sprintf(text, "%d/%d", g_Hud.m_GemCount, g_Hud.m_GemCount);
        mobys = g_HudMobys;
        vec.x = 90;
        vec.y = 36;
        vec.z = 2880;
#if GAME_VER == VER_NTSC_U
        func_80017FE4(text, &vec, 28, 11);
#else
        func_800181AC(text, &vec, nullptr, 28, 11);
#endif
        mobys--;
        for (i = 0; (int)g_HudMobys <= (int)mobys; i++) {
          mobys->m_Rotation.z =
              COSINE_8(g_Hud.m_GemSteadyTicks * 4 + (i * 12) & 0xFF) >> 7;
          *shaded_mobys++ = mobys;
          mobys--;
        }
      }
    }

    if (g_Hud.m_DragonDisplayState != 0) {
      for (i = 0; i < 3; i++) {
        *shaded_mobys++ = &g_Hud.m_Mobys[5 + i];
      }
    }

    if (g_Hud.m_LifeDisplayState != 0) {
      for (i = 0; i < 3; i++) {
        *shaded_mobys++ = &g_Hud.m_Mobys[8 + i];
      }
    }

    if (g_Hud.m_KeyDisplayState != 0 && g_Hud.m_KeyFlag == 1) {
      *shaded_mobys++ = &g_Hud.m_Mobys[11];
    }

    *shaded_mobys = nullptr;
  }

  if (g_Hud.m_LifeDisplayState != 0) {
    ColorInt col;
    for (i = 0; i < g_Hud.m_LifeOrbCount; i++) {
      col.r =
          (COSINE_8((g_RenderEffects.m_SpecularTime - i * 256 / 20) & 0xff) >>
           7) +
          128;
      col.g = col.r;
      col.b = col.r;

      func_8001919C(&g_Hud.m_SpriteRect[12 + i], &g_Hud.m_OrbAndEggSprite[0],
                    &col);
    }
  }

  if (g_Hud.m_EggDisplayState != 0) {
    for (i = 0; i < g_Hud.m_EggCount; i++) {
      func_8001919C(&g_Hud.m_SpriteRect[i],
                    &g_Hud.m_OrbAndEggSprite[(g_Hud.unk_0x40 + i) % 9 + 1], 0);
    }
  }
}

/// @brief Creates Mobys, shadows, Spyro, flame and glows and sparkles
void func_80019698(void) {
  func_8001F158();                // Initialize Moby renderer, perform culling
  Memset(&g_SonyImage, 0, 0x900); // Clear Moby queue buffer
  func_8001F798();                // Render regular Mobys
  func_800208FC();                // Initialize specular Moby renderer
  func_80020F34();                // Render specular Mobys
  func_80022A2C();                // Render shaded Mobys with lighting
  func_80059F8C();                // Render Moby shadows

  if (g_IsSpyroHidden == 0) {
    func_80023AC4(); // Render Spyro's model
    func_80059A48(); // Render Spyro's shadow
  }

  if (g_SpyroFlame.m_IsFlameActive != 0) {
    func_80058D64(); // Render Spyro's flame
  }

  func_80058BA8(); // Render glows and sparkles
}

/// @brief Creates the level transition text
void func_8001973C(void) {
#if GAME_VER == VER_NTSC_U
  const int titleAdvance = 8;
  const int textScale = 18;
  const int textDepth = 4352;
  const int letterSpacing = 16;
  const int letterDepth = 5120;
  const int textColor = 2;
  const int transitionEnd = 416;
  const int titleExitStart = 385;
  const int treasureExitStart = 384;
  const int totalTallyStart = 272;
#else
  const int titleAdvance = 7;
  const int textScale = 16;
  const int textDepth = 5120;
  const int letterSpacing = 14;
  const int letterDepth = 5632;
  const int textColor = 11;
  const int transitionEnd = 384;
  const int titleExitStart = 357;
  const int treasureExitStart = 356;
  const int totalTallyStart = 256;
#endif

  int levelIndex;
  Moby *mobys;
  Vector3D vec1;
  Vector3D vec2;
  char textbuffer[32];
  int i;

  levelIndex = (g_NextLevelId / 10 - 1) * 6 + (g_NextLevelId % 10);
  if (g_NextLevelId % 10 == 0) {
    #if GAME_VER == VER_NTSC_U
    sprintf(textbuffer, "RETURNING HOME...");
#else
    sprintf(textbuffer, StringReturningHome);
#endif
  } else if ((g_NextLevelId < 60 && g_NextLevelId % 10 == 4) ||
             (g_NextLevelId == 63)) {
#if GAME_VER == VER_NTSC_U
    sprintf(textbuffer, "CONFRONTING %s...", g_LevelNames[levelIndex]);
#else
    sprintf(textbuffer, "%s %s", StringConfronting,
            g_LevelNames[levelIndex]);
#endif
  } else {
#if GAME_VER == VER_NTSC_U
    sprintf(textbuffer, "ENTERING %s...", g_LevelNames[levelIndex]);
#else
    sprintf(textbuffer, "%s %s", StringEntering_Level,
            g_LevelNames[levelIndex]);
#endif
  }

  vec1.x = 256 - (strlen(textbuffer) - 1) * titleAdvance;

  if (g_LevelTransTicks < 32) {
    vec1.y = SINE_8(g_LevelTransTicks * 2) >> 7;
  } else if (g_LevelTransTicks >= titleExitStart) {
    vec1.y = SINE_8((transitionEnd - g_LevelTransTicks) * 2) >> 7;
  } else {
    vec1.y = 32;
  }
  vec1.z = textDepth;
  vec2.x = letterSpacing;
  vec2.y = 1;
  vec2.z = letterDepth;
  mobys = g_HudMobys;
  func_800181AC(textbuffer, &vec1, &vec2, textScale, textColor);
  mobys--;
  for (i = 0; (int)g_HudMobys <= (int)mobys; i++) {
    mobys->m_Rotation.z = COSINE_8(g_LevelTransTicks * 2 + i * 12 & 0xFF) >> 7;
    mobys--;
  }

  if (g_LevelTransTicks >= 64) {
    int center_offset, displayed_counter, tally_duration, tally_countdown;
    center_offset = 40;
    for (i = g_GemTotal; i >= 10; i = i / 10) {
      center_offset -= 10;
    }
#if GAME_VER == VER_NTSC_U
    vec1.x = center_offset + 40;
    vec1.z = 4352;
#endif
    if (g_LevelTransTicks < 96) {
      int a = g_LevelTransTicks - 64;
      vec1.y = 272 - (SINE_8(a * 2) >> 6);
    } else if (g_LevelTransTicks < g_LevelTransChestDuration) {
      vec1.y = 208;
    } else if (g_LevelTransTicks < g_LevelTransChestDuration + 32) {
      int a = g_LevelTransTicks - 32;
      vec1.y = 272 - (SINE_8((g_LevelTransChestDuration - a) * 2) >> 6);
    } else if (g_LevelTransTicks <= 223) {
      vec1.y = 272;
    } else if (g_LevelTransTicks < 0x100) {
      int a = g_LevelTransTicks - 224;
      vec1.y = 272 - (SINE_8(a * 2) >> 6);
    } else if (g_LevelTransTicks < treasureExitStart) {
      vec1.y = 208;
    } else if (g_LevelTransTicks < transitionEnd) {
      vec1.y = 272 - (SINE_8((transitionEnd - g_LevelTransTicks) * 2) >> 6);
    } else {
      vec1.y = 272;
    }
#if GAME_VER == VER_NTSC_U
    if (g_LevelTransTicks < 224) {
      sprintf(textbuffer, "TREASURE FOUND");
    } else {
      sprintf(textbuffer, "TOTAL TREASURE");
      vec1.x -= 16;
    }
    mobys = g_HudMobys;
    func_800181AC(textbuffer, &vec1, &vec2, textScale, textColor);
    mobys--;
#else
    mobys = g_HudMobys;
    vec1.y += 2;
    if (g_LevelTransTicks < 224) {
      vec1.x = StringTreasureFoundXOffset + center_offset;
      func_800181AC(StringTreasureFound, &vec1, &vec2,
                    textScale, textColor);
    } else {
      vec1.x = StringTotalTreasureXOffset + center_offset;
      func_800181AC(StringTotalTreasure, &vec1, &vec2,
                    textScale, textColor);
    }
    mobys--;
    vec1.y -= 2;
#endif
    for (i = 0; (int)g_HudMobys <= (int)mobys; i++) {
      mobys->m_Rotation.z =
          COSINE_8(g_LevelTransTicks * 2 + i * 12 & 0xFF) >> 7;
      mobys--;
    }
    if (g_LevelTransTicks < 128) {
      displayed_counter = g_LevelGemCount[g_PreviousLevelIndex] - D_8007587C;
    } else if (g_LevelTransTicks < 224) {
      tally_duration = (g_NGemsSinceLevelEntry + 1) * 2;
      if (tally_duration > 64) {
        tally_duration = 64;
      }
      tally_countdown = 128 + tally_duration - g_LevelTransTicks;
      if (tally_countdown < 0) {
        tally_countdown = 0;
      }
      if (tally_duration < tally_countdown) {
        tally_countdown = tally_duration;
      }
      displayed_counter =
          ((g_LevelGemCount[g_PreviousLevelIndex] - D_8007587C) *
           tally_countdown) /
          tally_duration;
    } else if (g_LevelTransTicks < totalTallyStart) {
      displayed_counter =
          g_GemTotal - (g_LevelGemCount[g_PreviousLevelIndex] - D_8007587C);
    } else {
      tally_duration = (g_NGemsSinceLevelEntry + 1) * 2;
      if (tally_duration > 64) {
        tally_duration = 64;
      }
      tally_countdown = totalTallyStart + tally_duration - g_LevelTransTicks;
      if (tally_countdown < 0) {
        tally_countdown = 0;
      }
      if (tally_duration < tally_countdown) {
        tally_countdown = tally_duration;
      }
      displayed_counter =
          g_GemTotal - (((g_LevelGemCount[g_PreviousLevelIndex] - D_8007587C) *
                         tally_countdown) /
                        tally_duration);
    }
    vec1.x = 376 + center_offset;
    vec1.z = 3968;
    sprintf(textbuffer, "%d", displayed_counter);
    mobys = g_HudMobys;
#if GAME_VER == VER_NTSC_U
    func_80017FE4(textbuffer, &vec1, 20, 2);
#else
    func_800181AC(textbuffer, &vec1, nullptr, 20, textColor);
#endif
    mobys--;
    for (i = 0; (int)g_HudMobys <= (int)mobys; i++) {
      mobys->m_Rotation.z =
          COSINE_8(g_LevelTransTicks * 2 + i * 12 & 0xFF) >> 7;
      mobys--;
    }
    for (i = 0; i < 32; i++) {
      if (g_LevelTransGems[i].active != 0) {
#if GAME_VER >= VER_C3_DEMO_NTSC_U
        int spread = (g_LevelTransGems[i].age >> 1) + 32;
        if (spread > 64) {
          spread = 128 - spread;
        }
#endif
        g_HudMobys--;
        Memset(g_HudMobys, 0, sizeof(Moby));
        g_HudMobys->m_Class = g_RecentGemsCollected[i];
#if GAME_VER == VER_NTSC_U
        g_HudMobys->m_Position.x =
            320 + center_offset + g_LevelTransGems[i].xOffset;
        g_HudMobys->m_Position.y = 198 - g_LevelTransGems[i].yOffset;
        g_HudMobys->m_Position.z = 0x1000;
#else
        g_HudMobys->m_Position.x =
            320 + center_offset +
            ((g_LevelTransGems[i].xOffset * SINE_8(g_LevelTransGems[i].age * 2) *
              spread) >> 18);
        g_HudMobys->m_Position.y =
            200 - ((g_LevelTransGems[i].yOffset *
                    SINE_8(g_LevelTransGems[i].age)) >> 12);
        g_HudMobys->m_Position.z =
            0x1000 + ((g_LevelTransGems[i]._04 *
                       SINE_8(g_LevelTransGems[i].age * 2) * spread) >> 18);
#endif
        g_HudMobys->m_Rotation.x = g_LevelTransGems[i].rot.x;
        g_HudMobys->m_Rotation.y = g_LevelTransGems[i].rot.y;
        g_HudMobys->m_Rotation.z = g_LevelTransGems[i].rot.z;
        g_HudMobys->m_DepthOffset = 0x7F;
        g_HudMobys->m_SpecularMetalType = g_HudMobys->m_Class - 0x52;
        g_HudMobys->m_RenderRadius = 0xFF;
      }
    }
    g_HudMobys--;
    Memset(g_HudMobys, 0, sizeof(Moby));
    g_HudMobys->m_Class = 473;
    g_HudMobys->m_Position.x = 320 + center_offset;
    if (g_LevelTransTicks < 224) {
      g_HudMobys->m_Position.y = vec1.y + 8;
      g_HudMobys->m_Position.z = 0x800;
      g_HudMobys->m_Rotation.x = 6;
      g_HudMobys->m_Rotation.z = 0xB0;
    } else {
      g_HudMobys->m_Position.y = vec1.y + 12;
      g_HudMobys->m_Position.z = 0x600;
      g_HudMobys->m_Rotation.x = 6;
      g_HudMobys->m_Rotation.z = 0xB0;
    }

    g_HudMobys->m_DepthOffset = 0x7F;
    g_HudMobys->m_SpecularMetalType = 11;
    g_HudMobys->m_RenderRadius = 0xFF;
  }
  g_SonyImage.m_ShadedMobys[0] = 0;
  func_80018880();
  func_80022A2C();
}

/// @brief Gamestate 1
void func_8001A050(void) {
  MATRIX A;
  MATRIX B;

  g_DB[0].m_DrawEnv.r0 = g_Cyclorama.m_BackgroundColor.r;
  g_DB[0].m_DrawEnv.g0 = g_Cyclorama.m_BackgroundColor.g;
  g_DB[0].m_DrawEnv.b0 = g_Cyclorama.m_BackgroundColor.b;
  g_DB[1].m_DrawEnv.r0 = g_Cyclorama.m_BackgroundColor.r;
  g_DB[1].m_DrawEnv.g0 = g_Cyclorama.m_BackgroundColor.g;
  g_DB[1].m_DrawEnv.b0 = g_Cyclorama.m_BackgroundColor.b;
  Memset(&g_SonyImage, 0, 0x900);
  if (g_LevelTransHudActive != 0) {
    func_8001973C();
  }
  func_80023AC4();
  if (g_Cyclorama.m_SectorCount != 0) {
    D_80075910 -= 2;
    if (D_80075910 < 0) {
      D_80075910 = 0;
    }
    if (D_80075910 != 0) {
      Memset(&A, 0, 0x20);
      SetTransMatrix(&A);
      A.m[0][0] = 4096;
      A.m[1][1] = Cos(g_Camera.m_Rotation.y - D_80075910);
      A.m[2][1] = Sin(g_Camera.m_Rotation.y - D_80075910);
      A.m[1][2] = -Sin(g_Camera.m_Rotation.y - D_80075910);
      A.m[2][2] = Cos(g_Camera.m_Rotation.y - D_80075910);
      Memset(&B, 0, 0x20);
      B.m[0][0] = Cos(g_Camera.m_Rotation.z);
      B.m[2][0] = -Sin(g_Camera.m_Rotation.z);
      B.m[1][1] = 4096;
      B.m[0][2] = Sin(g_Camera.m_Rotation.z);
      B.m[2][2] = Cos(g_Camera.m_Rotation.z);
      MulMatrix(&A, (MATRIX *)&B);
      Memset(&B, 0, 0x20);
      B.m[0][0] = Cos(g_Camera.m_Rotation.x);
      B.m[1][0] = -Sin(g_Camera.m_Rotation.x);
      B.m[0][1] = Sin(g_Camera.m_Rotation.x);
      B.m[1][1] = Cos(g_Camera.m_Rotation.x);
      B.m[2][2] = 4096;
      MulMatrix(&A, (MATRIX *)&B);
      Memcpy(&B, &A, 0x14);
      B.m[1][0] = (A.m[1][0] * 320) / 512;
      B.m[1][1] = (A.m[1][1] * 320) / 512;
      B.m[1][2] = (A.m[1][2] * 320) / 512;
      func_8004EBA8(-1, (SHORTMATRIX *)&A, (SHORTMATRIX *)&B);
    } else {
      func_8004EBA8(-1, &g_Camera.m_ViewMatrix, &g_Camera.m_ProjectionMatrix);
    }
  }
  DrawSync(0);

  if (D_80075784) {
    VSync(0);
  }

  D_80075950.pre = VSync(-1);

  while (D_80075950.pre - D_80075950.post < 2) {
    VSync(0);
    D_80075950.pre = VSync(-1);
  }

  D_80075950.post = VSync(-1);

  PutDispEnv(&g_CurDB->m_DispEnv);
  PutDrawEnv(&g_CurDB->m_DrawEnv);
  DrawOTag(func_80016784(0x800));
}

extern int D_80075720; // Selected menu item index
extern int D_80075744; // The index of the current page of the inventory screen
extern int D_800757C8; // OptionsSubmenuIsOpen
extern int D_800757CC; // Transition progress between inventory pages.
extern int D_800758B8; // Pause menu text rotation ticks
extern int D_8007568C; // Pause menu no button ticks

int CalculateCompletion(void);

/// @brief Gamestate 2 & 3
void func_8001A40C(void) {

  int i;
  DR_MODE *drm;
  POLY_F4 *f4;
  POLY_FT4 *ft4;
  LINE_F2 *l2;

  RECT rcStore;
  char szBuf[32];
  Vector3D position;
#if GAME_VER == VER_NTSC_U
  Vector3D quitMenuSpacing;
#endif
  Vector3D optionMenuSpacing;
  Vector3D vec4;
  RECT rc1;
  RECT rc2;
  Vector3D levelNameSpacing;

  int worldNameLen;
  int volumeBarCt;
  Moby *curMoby;
  Moby *endMoby;
  u_char mobyCount;
  int level_y;
  int world_lt_3_offset;
  int levelGemCount;
  u_char act_avail;

  if (D_800758B8 == 0) {
    func_800521C0();
    func_80019698();
    func_800573C8();
    func_80050BD0();
    func_8002B9CC();
    DrawSync(0);
    VSync(0);
    PutDispEnv(&g_CurDB->m_DispEnv);
    PutDrawEnv(&g_CurDB->m_DrawEnv);
    DrawOTag(func_80016784(0x800));
    DrawSync(0);
    VSync(0);
    PutDispEnv(&g_CurDB->m_DispEnv);
    setRECT(&rcStore, 512, 0, 256, 225);
    StoreImage(&rcStore, g_Buffers.m_HudOTStart - 0x1c200);

    for (i = 0; i < 4; ++i) {
      setRECT(&rcStore, i * 128, g_CurDB != g_DB ? 248 : 8, 128, 224);
      StoreImage(&rcStore, g_Buffers.m_LowerPolyBuffer);
      DrawSync(0);
      func_80017E98(g_Buffers.m_LowerPolyBuffer, 0x7000);
      setRECT(&rcStore, 512 + i * 64, 0, 64, 224);
      LoadImage(&rcStore, g_Buffers.m_LowerPolyBuffer);
    }

    setRECT(&rcStore, 512, 224, 32, 1);
    LoadImage(&rcStore, (u_long *)g_GrayscalePalette);
    DrawSync(0);
    D_80075950.post = VSync(-1);
  } else {
    if (g_CurDB == &g_DB[0]) {
      PutDrawEnv(&g_DB[1].m_DrawEnv);
    } else {
      PutDrawEnv(&g_DB[0].m_DrawEnv);
    }

    D_800758B0 = 0;
    D_800757B0 = g_Buffers.m_LowerPolyBuffer;
    D_80075780 = g_Buffers.m_LowerPolyBuffer + 0x1c000;
    g_HudMobys = D_800756FC = g_Buffers.m_HudOTStart - 0x1c200;

    for (i = 0; i < 4; i++) {
      ft4 = D_800757B0;
      ft4->tag = PRIM_TAG(POLY_FT4);
      ft4->code = PRIM_POLY_FT4;
      setRGB0(ft4, 0x4C, 0x80, 0x40);
      setXY4(ft4, i * 128, 8, ft4->x0 + 128, ft4->y0, ft4->x0,
             ft4->y0 + 224 - 1, ft4->x0 + 128, ft4->y0 + 224 - 1);
      setUV4(ft4, 0, 0, ft4->u0 + 128, ft4->v0, ft4->u0, ft4->v0 + 224 - 1,
             ft4->u0 + 128, ft4->v0 + 224 - 1);
      ft4->clut = getClut(512, 224);
      ft4->tpage = getTPage(1, 0, 512, 0) + i;
      func_800168DC(ft4);
      D_800757B0 = ft4 + 1;
    }

    l2 = D_800757B0;
    l2->tag = PRIM_TAG(LINE_F2);
    l2->code = PRIM_LINE_F2;
    setXY2(l2, 0, 231, 512, 231);
    setRGB0(l2, 0, 0, 0);
    func_800168DC(l2);
    D_800757B0 = l2 + 1;

    if (g_Gamestate == 2) {

      // pause menu
      drm = D_800757B0;

      SetDrawMode(drm, 1, 0, getTPage(0, 2, 0, 0), nullptr);
      func_800168DC(drm);
      f4 = (POLY_F4 *)(drm + 1);
      f4->tag = PRIM_TAG(POLY_F4);
      f4->code = PRIM_STP(PRIM_POLY_F4);
      setRGB0(f4, 64, 64, 64);
      f4->y0 = 67;
      f4->y1 = 67;
#if GAME_VER == VER_NTSC_U
      if (D_800757C8 == 1) {
        act_avail = (g_ActAvailable != 0);
        f4->x0 = 84;
        f4->x1 = 428;
        f4->y2 = 182 + act_avail * 16;
        f4->y3 = 182 + act_avail * 16;
      } else {
        act_avail = 1;
        f4->x0 = 140;
        f4->x1 = 372;
        f4->y2 = 176;
        f4->y3 = 176;
      }
#else
      if (D_800757C8 == 1) {
        act_avail = (g_ActAvailable != 0);
        f4->x0 = 82;
        f4->x1 = 430;
        f4->y2 = 161 + act_avail * 18;
        f4->y3 = 161 + act_avail * 18;
      } else {
        act_avail = g_LevelId % 10 != 0;
        f4->x0 = 112;
        f4->x1 = 400;
        f4->y2 = 158 + act_avail * 18;
        f4->y3 = 158 + act_avail * 18;
      }
#endif
      f4->x2 = f4->x0;
      f4->x3 = f4->x1;
      func_800168DC(f4);
      D_800757B0 = f4 + 1;

#if GAME_VER == VER_NTSC_U
      func_8001844C(224, 97, 288, 97);
      if (D_800757C8 == 1) {
        func_8001844C(84, 67, 428, 67);
        func_8001844C(428, 67, 428, 182 + act_avail * 16);
        func_8001844C(428, 182 + act_avail * 16, 84, 182 + act_avail * 16);
        func_8001844C(84, 182 + act_avail * 16, 84, 67);
      } else {
        func_8001844C(140, 67, 372, 67);
        func_8001844C(372, 67, 372, 158 + act_avail * 18);
        func_8001844C(372, 158 + act_avail * 18, 140, 158 + act_avail * 18);
        func_8001844C(140, 158 + act_avail * 18, 140, 67);
      }
      setXYZ(&position, 186, 82, 3072);
      func_80017FE4("PAUSED", &position, 28, 11); // PAUSED

#else
      if (D_800757C8 == 1) {
        func_8001844C(82, 67, 430, 67);
        func_8001844C(430, 67, 430, 161 + act_avail * 18);
        func_8001844C(430, 161 + act_avail * 18, 82, 161 + act_avail * 18);
        func_8001844C(82, 161 + act_avail * 18, 82, 67);
      } else {
        func_8001844C(224, 97, 288, 97);
        func_8001844C(112, 67, 400, 67);
        func_8001844C(400, 67, 400, 158 + act_avail * 18);
        func_8001844C(400, 158 + act_avail * 18, 112, 158 + act_avail * 18);
        func_8001844C(112, 158 + act_avail * 18, 112, 67);
        setXYZ(&position, StringPausedX, 82, 3072);
        func_800181AC(StringPaused, &position, nullptr, 28, 11);
      }

#endif
#if GAME_VER == VER_NTSC_U
      if (D_800757C8 == 2) {
        // quit menu
        setXYZ(&quitMenuSpacing, 16, 1, 0x1400);
        setXYZ(&position, 183, 119, 0x1100);
        func_800181AC("QUIT GAME?", &position, &quitMenuSpacing, 18, 11);

        setXYZ(&position, 192, 148, 0x1100);
        func_800181AC("YES", &position, &quitMenuSpacing, 18, 11);
        if (D_80075720 == 0) {
          curMoby = g_HudMobys;
          mobyCount = 3;
        }

        setXYZ(&position, 296, 148, 0x1100);
        func_800181AC("NO", &position, &quitMenuSpacing, 18, 11);
        if (D_80075720 == 1) {
          curMoby = g_HudMobys;
          mobyCount = 2;
        }

        for (i = 0; i < mobyCount; ++i) {
          (*curMoby++).m_Rotation.z =
              COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
        }
      } else if (D_800757C8 == 1) {
#else
      if (D_800757C8 == 1) {
#endif
        // options menu
#if GAME_VER >= VER_C3_DEMO_NTSC_U
        endMoby = g_HudMobys;
#endif
        setXYZ(&optionMenuSpacing, 15, 1, 0x1600);
#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 107, 108, 0x1400);
        func_800181AC("SOUND EFFECTS", &position, &optionMenuSpacing, 16, 11);
#else
        position.z = 0x1400;
        position.y = 79;
        position.x = StringSoundEffectsX;
        func_800181AC(StringSoundEffects,
                      &position, &optionMenuSpacing, 16, 11);
#endif
        if (D_80075720 == 0) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          mobyCount = 11;
#else
          mobyCount = endMoby - curMoby;
#endif
        }

#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 121, 124, 0x1400);
        func_800181AC("MUSIC VOLUME", &position, &optionMenuSpacing, 16, 11);
#else
        endMoby = g_HudMobys;
        position.y = 97;
        position.x = StringMusicVolumeX;
        func_800181AC(StringMusicVolume,
                      &position, &optionMenuSpacing, 16, 11);
#endif
        if (D_80075720 == 1) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          mobyCount = 11;
#else
          mobyCount = endMoby - curMoby;
#endif
        }

#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 107, 140, 0x1400);
        func_800181AC("SPEAKER SETUP", &position, &optionMenuSpacing, 16, 11);
#else
        endMoby = g_HudMobys;
        position.y = 115;
        position.x = StringSpeakerSetupX;
        func_800181AC(StringSpeakerSetup,
                      &position, &optionMenuSpacing, 16, 11);
#endif
        if (D_80075720 == 2) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          mobyCount = 12;
#else
          mobyCount = endMoby - curMoby;
#endif
        }

        if (g_ActAvailable) {
#if GAME_VER == VER_NTSC_U
          setXYZ(&position, 163, 156, 0x1400);
          func_800181AC("VIBRATION", &position, &optionMenuSpacing, 16, 11);
#else
          endMoby = g_HudMobys;
          position.y = 133;
          position.x = StringVibrationX;
          func_800181AC(StringVibration,
                        &position, &optionMenuSpacing, 16, 11);
#endif
          if (D_80075720 == 3) {
            curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
            mobyCount = 9;
#else
            mobyCount = endMoby - curMoby;
#endif
          }
        }

#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 208, act_avail * 16 + 156, 0x1100);
        func_800181AC("CAMERA", &position, &optionMenuSpacing, 16, 11);
#else
        endMoby = g_HudMobys;
        position.y = act_avail * 18 + 133;
        position.x = StringCameraX;
        func_800181AC(StringCamera,
                      &position, &optionMenuSpacing, 16, 11);
#endif
        if (D_80075720 == 4) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          mobyCount = 6;
#else
          mobyCount = endMoby - curMoby;
#endif
        }

#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 238, act_avail * 16 + 172, 0x1100);
        func_800181AC("DONE", &position, &optionMenuSpacing, 16, 11);
#else
        endMoby = g_HudMobys;
        position.y = act_avail * 18 + 151;
        position.x = StringDoneX;
        func_800181AC(StringDone,
                      &position, &optionMenuSpacing, 16, 11);
#endif
        if (D_80075720 == 5) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          mobyCount = 4;
#else
          mobyCount = endMoby - curMoby;
#endif
        }

        for (i = 0; i < mobyCount; ++i) {
          (*curMoby++).m_Rotation.z =
              COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
        }

        // sound volume bars
        if (D_80075754 == 0) {
#if GAME_VER == VER_NTSC_U
          setXYZ(&position, 322, 108, 0x1400);
          func_800181AC("OFF", &position, &optionMenuSpacing, 16, 11);
#else
          endMoby = g_HudMobys;
          position.x = 324;
          position.y = 79;
          func_800181AC(StringOff, &position,
                        &optionMenuSpacing, 16, 11);
#endif
          if (D_80075720 == 0) {
            curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
            for (i = 0; i < 3; ++i) {
#else
            mobyCount = endMoby - curMoby;
            for (i = 0; i < mobyCount; ++i) {
#endif
              (*curMoby++).m_Rotation.z =
                  COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
            }
          }
        } else {
          for (i = 0; i < D_80075754; ++i) {
            --g_HudMobys;
            Memset(g_HudMobys, 0, sizeof(Moby));
            g_HudMobys->m_Class = MOBYCLASS_LETTER_I; // nice...
#if GAME_VER == VER_NTSC_U
            g_HudMobys->m_Position.x = 320 + i * 10;
#else
            g_HudMobys->m_Position.x = 322 + i * 10;
#endif
#if GAME_VER == VER_NTSC_U
            g_HudMobys->m_Position.y = 109;
#else
            g_HudMobys->m_Position.y = 80;
#endif
            g_HudMobys->m_Position.z = 0x1400;
            if (D_80075720 == 0) {
              g_HudMobys->m_Rotation.z =
                  COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
            } else {
              g_HudMobys->m_Rotation.z = 0;
            }
            g_HudMobys->m_DepthOffset = 127;
            g_HudMobys->m_SpecularMetalType = 11;
            g_HudMobys->m_RenderRadius = 0xFF;
          }
        }

        // music volume bars
        if (D_80075748 == 0) {
#if GAME_VER == VER_NTSC_U
          setXYZ(&position, 322, 124, 0x1400);
          func_800181AC("OFF", &position, &optionMenuSpacing, 16, 11);
#else
          endMoby = g_HudMobys;
          position.x = 324;
          position.y = 97;
          func_800181AC(StringOff, &position,
                        &optionMenuSpacing, 16, 11);
#endif
          if (D_80075720 == 1) {
            curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
            for (i = 0; i < 3; ++i) {
#else
            mobyCount = endMoby - curMoby;
            for (i = 0; i < mobyCount; ++i) {
#endif
              (*curMoby++).m_Rotation.z =
                  COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
            }
          }
        } else {
          for (i = 0; i < D_80075748; ++i) {
            --g_HudMobys;
            Memset(g_HudMobys, 0, sizeof(Moby));
            g_HudMobys->m_Class = MOBYCLASS_LETTER_I; // nice...
#if GAME_VER == VER_NTSC_U
            g_HudMobys->m_Position.x = 320 + i * 10;
#else
            g_HudMobys->m_Position.x = 322 + i * 10;
#endif
#if GAME_VER == VER_NTSC_U
            g_HudMobys->m_Position.y = 125;
#else
            g_HudMobys->m_Position.y = 98;
#endif
            g_HudMobys->m_Position.z = 0x1400;
            if (D_80075720 == 1) {
              g_HudMobys->m_Rotation.z =
                  COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
            } else {
              g_HudMobys->m_Rotation.z = 0;
            }
            g_HudMobys->m_DepthOffset = 127;
            g_HudMobys->m_SpecularMetalType = 11;
            g_HudMobys->m_RenderRadius = 0xFF;
          }
        }

#if GAME_VER == VER_NTSC_U
        // audio type
        setXYZ(&position, 322, 140, 0x1400);
        if (g_Spu.m_AudioMono != 0) {
          func_800181AC("MONO", &position, &optionMenuSpacing, 16, 11);
          volumeBarCt = 4;
        } else {
          func_800181AC("STEREO", &position, &optionMenuSpacing, 16, 11);
          volumeBarCt = 6;
        }
#else
        endMoby = g_HudMobys;
        position.x = 324;
        position.y = 115;
        if (g_Spu.m_AudioMono != 0) {
          func_800181AC(StringMono,
                        &position, &optionMenuSpacing, 16, 11);
        } else {
          func_800181AC(StringStereo,
                        &position, &optionMenuSpacing, 16, 11);
        }
#endif
        if (D_80075720 == 2) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          for (i = 0; i < volumeBarCt; ++i) {
#else
          mobyCount = endMoby - curMoby;
          for (i = 0; i < mobyCount; ++i) {
#endif
            (*curMoby++).m_Rotation.z =
                COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
          }
        }

        if (g_ActAvailable) {
#if GAME_VER >= VER_C3_DEMO_NTSC_U
          endMoby = g_HudMobys;
#endif
#if GAME_VER == VER_NTSC_U
          setXYZ(&position, 322, 156, 0x1400);
          if (g_ActEnabled != 0) {
            func_800181AC("ON", &position, &optionMenuSpacing, 16, 11);
            volumeBarCt = 2;
          } else {
            func_800181AC("OFF", &position, &optionMenuSpacing, 16, 11);
            volumeBarCt = 3;
          }

#else
          position.x = 324;
          position.y = 133;
          if (g_ActEnabled != 0) {
            func_800181AC(StringOn,
                          &position, &optionMenuSpacing, 16, 11);
          } else {
            func_800181AC(StringOff,
                          &position, &optionMenuSpacing, 16, 11);
          }
#endif
          if (D_80075720 == 3) {
            curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
            for (i = 0; i < volumeBarCt; ++i) {
#else
            mobyCount = endMoby - curMoby;
            for (i = 0; i < mobyCount; ++i) {
#endif
              (*curMoby++).m_Rotation.z =
                  COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
            }
          }
        }

#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 322, act_avail * 16 + 156, 0x1400);
        if (D_80075914 == 2) {
          func_800181AC("ACTIVE", &position, &optionMenuSpacing, 16, 11);
          volumeBarCt = 6;
        } else {
          func_800181AC("PASSIVE", &position, &optionMenuSpacing, 16, 11);
          volumeBarCt = 7;
        }
#else
        endMoby = g_HudMobys;
        position.x = 324;
        position.y = act_avail * 18 + 133;
        if (D_80075914 == 2) {
          func_800181AC(StringActive,
                        &position, &optionMenuSpacing, 16, 11);
        } else {
          func_800181AC(StringPassive,
                        &position, &optionMenuSpacing, 16, 11);
        }
#endif
        if (D_80075720 == 4) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          for (i = 0; i < volumeBarCt; ++i) {
#else
          mobyCount = endMoby - curMoby;
          for (i = 0; i < mobyCount; ++i) {
#endif
            (*curMoby++).m_Rotation.z =
                COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
          }
        }
      } else {
        setXYZ(&vec4, 16, 1, 0x1400);
#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 199, 110, 0x1100);
        func_800181AC("CONTINUE", &position, &vec4, 18, 11);
#else
        endMoby = g_HudMobys;
        setXYZ(&position, StringContinue_PauseX, 110, 0x1100);
        func_800181AC(StringContinue_Pause, &position, &vec4, 18, 11);
#endif
        if (D_80075720 == 0) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          mobyCount = 8;
#else
          mobyCount = endMoby - curMoby;
#endif
        }

#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 207, 128, 0x1100);
        func_800181AC("OPTIONS", &position, &vec4, 18, 11);
#else
        endMoby = g_HudMobys;
        setXYZ(&position, StringOptionsX, 128, 0x1100);
        func_800181AC(StringOptions, &position, &vec4, 18, 11);
#endif
        if (D_80075720 == 1) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          mobyCount = 7;
#else
          mobyCount = endMoby - curMoby;
#endif
        }

#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 191, 146, 0x1100);
        func_800181AC("INVENTORY", &position, &vec4, 18, 11);
#else
        endMoby = g_HudMobys;
        setXYZ(&position, StringInventoryX, 146, 0x1100);
        func_800181AC(StringInventory, &position, &vec4, 18, 11);
#endif
        if (D_80075720 == 2) {
          curMoby = g_HudMobys;
#if GAME_VER == VER_NTSC_U
          mobyCount = 9;
#else
          mobyCount = endMoby - curMoby;
#endif
        }

#if GAME_VER == VER_NTSC_U
        if (g_IsFlightLevel != 0) {
          // flight level
          setXYZ(&position, 231, 164, 0x1100);
          func_800181AC("QUIT", &position, &vec4, 18, 11);
          if (D_80075720 == 3) {
            curMoby = g_HudMobys;
            mobyCount = 4;
          }
        } else {
          // not flight level
          if (g_LevelId != (g_LevelId / 10) * 10) {
            setXYZ(&position, 183, 164, 0x1100);
            func_800181AC("EXIT LEVEL", &position, &vec4, 18, 11);
            if (D_80075720 == 3) {
              curMoby = g_HudMobys;
              mobyCount = 9;
            }
          } else {
            setXYZ(&position, 191, 164, 0x1100);
            func_800181AC("QUIT GAME", &position, &vec4, 18, 11);
            if (D_80075720 == 3) {
              curMoby = g_HudMobys;
              mobyCount = 8;
            }
          }
        }

#else
        if (g_IsFlightLevel != 0) {
          endMoby = g_HudMobys;
          setXYZ(&position, StringQuit_PauseX, 164, 0x1100);
          func_800181AC(StringQuit_Pause, &position, &vec4, 18, 11);
          if (D_80075720 == 3) {
            curMoby = g_HudMobys;
            mobyCount = endMoby - curMoby;
          }
        } else if (g_LevelId != (g_LevelId / 10) * 10) {
          endMoby = g_HudMobys;
          setXYZ(&position, StringExitLevelX, 164, 0x1100);
          func_800181AC(StringExitLevel, &position, &vec4, 18, 11);
          if (D_80075720 == 3) {
            curMoby = g_HudMobys;
            mobyCount = endMoby - curMoby;
          }
        }

#endif
        for (i = 0; i < mobyCount; ++i) {
          (*curMoby++).m_Rotation.z =
              COSINE_8((D_800758B8 * 8 + i * 12) & 0xFF) * 3 >> 9;
        }
      }
    } else if (g_Gamestate == 3) {
      int temp;
      // inventory menu
      int completion = CalculateCompletion();
      setXYZ(&position, 440, 44, 0x900);
      if (completion > 9) {
        position.x = 406;
      }
      if (completion > 99) {
        position.x -= 34;
      }

      // completion pct (top right)
      sprintf(szBuf, "%d", completion);
#if GAME_VER == VER_NTSC_U
      func_80017FE4(szBuf, &position, 34, 2);
#else
      func_800181AC(szBuf, &position, nullptr, 34, 11);
#endif
      position.x = 478;
      szBuf[0] = '%';
      szBuf[1] = '\0';
#if GAME_VER == VER_NTSC_U
      func_80017FE4(szBuf, &position, 34, 2);
#else
      func_800181AC(szBuf, &position, nullptr, 34, 11);
#endif

      // gem total (top left)
      setXYZ(&position, 90, 36, 0xB40);
      sprintf(szBuf, "%d", g_GemTotal);
#if GAME_VER == VER_NTSC_U
      func_80017FE4(szBuf, &position, 27, 2);
#else
      func_800181AC(szBuf, &position, nullptr, 27, 11);
#endif

      // dragon total (top center)
      setXYZ(&position, 333, 36, 0xB40);
      if (g_DragonTotal > 9)
        position.x = 306;
      sprintf(szBuf, "%d", g_DragonTotal);
#if GAME_VER == VER_NTSC_U
      func_80017FE4(szBuf, &position, 27, 2);
#else
      func_800181AC(szBuf, &position, nullptr, 27, 11);
#endif

      // gem chest icon (top left)
      --g_HudMobys;
      Memset(g_HudMobys, 0, sizeof(Moby));
      g_HudMobys->m_Class = MOBYCLASS_HUD_GEM_CHEST;
      setXYZ(&g_HudMobys->m_Position, 46, 46, 0xC00);
      g_HudMobys->m_Rotation.y = COSINE_8(g_Hud.unk_0x3c) >> 9;
      g_HudMobys->m_Rotation.z = g_Hud.unk_0x3c;
      g_HudMobys->m_DepthOffset = 127;
      g_HudMobys->m_SpecularMetalType = 11;
      g_HudMobys->m_RenderRadius = 0xFF;

      // dragon icon (top center)
      --g_HudMobys;
      Memset(g_HudMobys, 0, sizeof(Moby));
      g_HudMobys->m_Class = MOBYCLASS_HUD_DRAGON;
      g_HudMobys->m_Position.x = 300;
      if (g_DragonTotal > 9)
        g_HudMobys->m_Position.x = 273;
      g_HudMobys->m_Position.y = 46;
      g_HudMobys->m_Position.z = 0x780;
      g_HudMobys->m_Rotation.z = COSINE_8((D_800758B8 * 8) & 0xFF) >> 8;
      g_HudMobys->m_DepthOffset = 127;
      g_HudMobys->m_SpecularMetalType = 11;
      g_HudMobys->m_RenderRadius = 0xFF;

      // egg sprites (top)
      if (g_EggTotal > 0) {

        for (i = 0; i < 12; ++i) {
          setRECT(&rc1, i * 27 + 24, 50, 24, 16);
          if (i >= g_EggTotal) {
            setXYZ(&position, 36 + i * 27, 58, 0x1800);
#if GAME_VER == VER_NTSC_U
            func_80017FE4("X", &position, 0, 2);
#else
            func_800181AC("X", &position, nullptr, 0, 11);
#endif
            setXYZ(&position, 32, 32, 32);
            func_8001919C(&rc1, &g_Hud.m_OrbAndEggSprite[1],
                          (ColorInt *)&position);
          } else {
            func_8001919C(
                &rc1, &g_Hud.m_OrbAndEggSprite[(g_Hud.unk_0x40 + i) % 9 + 1],
                nullptr);
          }
        }
      }
      func_8001844C(64, 69, 448, 69);
      drm = D_800757B0;
      SetDrawMode(drm, 1, 0, getTPage(0, 2, 0, 0), nullptr);
      func_800168DC(drm);

      f4 = (POLY_F4 *)(drm + 1);
      f4->tag = PRIM_TAG(POLY_F4);
      f4->code = PRIM_STP(PRIM_POLY_F4);
      setRGB0(f4, 96, 96, 96);
      setXY4(f4, 64, 70, 448, 70, 64, 71, 448, 71);
      func_800168DC(f4);
      D_800757B0 = f4 + 1;

      func_8001844C(64, 71, 448, 71);
      if (D_80075744 >= 1 && g_VisitedFlags[D_80075744 * 6 - 6]) {
        setXYZ(&position, 40, 72, 0x1100);
        func_80018534(&position, D_800758B8 * 2, 1);
      }
      if (D_80075744 <= 4 && g_VisitedFlags[D_80075744 * 6 + 6]) {
        setXYZ(&position, 471, 72, 0x1100);
        func_80018534(&position, D_800758B8 * 2, 0);
      }

      // world name title (above table)
      if (D_80075744 < 3) {
        world_lt_3_offset = 8;
      } else {
        world_lt_3_offset = 0;
      }

#if GAME_VER == VER_NTSC_U
      worldNameLen = (10 * (strlen(g_HomeworldNames[D_80075744]) - 1) - 158);
      setXYZ(&position, D_800757CC - worldNameLen, 92, 3584);
      setXYZ(&levelNameSpacing, 20, 2, 0x1000);
      func_800181AC(g_HomeworldNames[D_80075744], &position, &levelNameSpacing,
                    23, 2);
      endMoby = g_HudMobys;
      for (i = 0; i < g_HomeworldNamesLength[D_80075744]; ++i) {
        endMoby->m_Rotation.z =
            COSINE_8(D_800758B8 * 8 + i * 12 & 0xFF) * 3 >> 9;
        endMoby++;
      }

#else
      position.x = D_800757CC + 170;
      for (i = 0; g_HomeworldNames[D_80075744][i] != 0; i++) {
        if (g_HomeworldNames[D_80075744][i] < 91) {
          position.x -= 9;
        }
      }
      position.y = 92;
      position.z = 3584;
      setXYZ(&levelNameSpacing, 18, 2, 0x1100);
      endMoby = g_HudMobys;
      func_800181AC(g_HomeworldNames[D_80075744], &position, &levelNameSpacing,
                    21, 11);
      endMoby--;
      for (i = 0; (int)endMoby >= (int)g_HudMobys; i++) {
        endMoby->m_Rotation.z =
            COSINE_8(D_800758B8 * 8 + i * 12 & 0xFF) >> 7;
        endMoby--;
      }

#endif
      // gem chest (above table)

      --g_HudMobys;
      Memset(g_HudMobys, 0, sizeof(Moby));
      g_HudMobys->m_Class = MOBYCLASS_HUD_GEM_CHEST;
#if GAME_VER == VER_NTSC_U
      temp = world_lt_3_offset + 346;
#else
      temp = world_lt_3_offset + 348;
#endif
      setXYZ(&g_HudMobys->m_Position, D_800757CC + temp, 108, 0xC00);
      g_HudMobys->m_Rotation.y = COSINE_8(g_Hud.unk_0x3c) >> 9;
      g_HudMobys->m_Rotation.z = g_Hud.unk_0x3c;
      g_HudMobys->m_DepthOffset = 127;
      g_HudMobys->m_SpecularMetalType = 11;
      g_HudMobys->m_RenderRadius = 0xFF;

      // dragon icon (above table)
      --g_HudMobys;
      Memset(g_HudMobys, 0, sizeof(Moby));
      g_HudMobys->m_Class = MOBYCLASS_HUD_DRAGON;
      i = 0;
#if GAME_VER == VER_NTSC_U
      setXYZ(&g_HudMobys->m_Position, D_800757CC + 465, 110, 0x780);
#else
      setXYZ(&g_HudMobys->m_Position, D_800757CC + 467, 110, 0x780);
#endif
      g_HudMobys->m_Rotation.z = COSINE_8((D_800758B8 * 8) & 0xFF) >> 8;
      g_HudMobys->m_DepthOffset = 127;
      g_HudMobys->m_SpecularMetalType = 11;
      g_HudMobys->m_RenderRadius = 0xFF;
      level_y = 123;
      for (i = 0; i < 6; ++i) {
        if (g_VisitedFlags[D_80075744 * 6 + i]) {
          // shaded background
          drm = D_800757B0;
          SetDrawMode(drm, 1, 0, getTPage(0, 2, 0, 0), nullptr);
          func_800168DC(drm);
          f4 = (POLY_F4 *)(drm + 1);
          f4->tag = PRIM_TAG(POLY_F4);
          f4->code = PRIM_STP(PRIM_POLY_F4);
          setRGB0(f4, 40, 40, 40);
          setXY4(f4, D_800757CC, level_y - 10, D_800757CC + 511, level_y - 10,
                 D_800757CC, level_y + 9, D_800757CC + 511, level_y + 9);
          func_800168DC(f4);
          D_800757B0 = f4 + 1;

          // 3 border lines between level stats
          func_8001844C(D_800757CC + 112, level_y - 11, D_800757CC + 208,
                        level_y - 11);
#if GAME_VER == VER_NTSC_U
          func_8001844C(D_800757CC + 314 + world_lt_3_offset, level_y - 11,
                        D_800757CC + 378 + world_lt_3_offset, level_y - 11);
#else
          func_8001844C(D_800757CC + 316 + world_lt_3_offset, level_y - 11,
                        D_800757CC + 380 + world_lt_3_offset, level_y - 11);
#endif
#if GAME_VER == VER_NTSC_U
          func_8001844C(D_800757CC + 430, level_y - 11, D_800757CC + 494,
                        level_y - 11);
#else
          func_8001844C(D_800757CC + 432, level_y - 11, D_800757CC + 496,
                        level_y - 11);
#endif

          endMoby = g_HudMobys;

          // level stats - level name
#if GAME_VER == VER_NTSC_U
          setXYZ(&position, D_800757CC + 36, level_y, 0x1100);
          setXYZ(&levelNameSpacing, 16, 1, 0x1400);
          func_800181AC(g_LevelNames[D_80075744 * 6 + i], &position,
                        &levelNameSpacing, 18, 2);
#else
          setXYZ(&position, D_800757CC + 26, level_y, 0x1400);
          setXYZ(&levelNameSpacing, 14, 1, 0x1600);
          func_800181AC(g_LevelNames[D_80075744 * 6 + i], &position,
                        &levelNameSpacing, 16, 11);
#endif

          // level stats - gem count

#if GAME_VER == VER_NTSC_U
          position.x = 0x113 + D_800757CC + world_lt_3_offset;
#else
          position.x = 0x115 + D_800757CC + world_lt_3_offset;
#endif

          if (g_LevelGemCount[D_80075744 * 6 + i] < 1000)
            position.x += 18;
          if (g_LevelGemCount[D_80075744 * 6 + i] < 100)
            position.x += 18;
          if (g_LevelGemCount[D_80075744 * 6 + i] < 10)
            position.x += 18;
          sprintf(szBuf, "%d", g_LevelGemCount[D_80075744 * 6 + i]);
#if GAME_VER == VER_NTSC_U
          func_80017FE4(szBuf, &position, 18, 2);
#else
          func_800181AC(szBuf, &position, nullptr, 18, 11);
#endif
#if GAME_VER == VER_NTSC_U
          position.x = D_800757CC + 346 + world_lt_3_offset;
#else
          position.x = D_800757CC + 348 + world_lt_3_offset;
#endif
          position.z = 0x1800;
#if GAME_VER == VER_NTSC_U
          func_80017FE4("/", &position, 18, 2);
#else
          func_800181AC("/", &position, nullptr, 18, 11);
#endif
#if GAME_VER == VER_NTSC_U
          position.x = D_800757CC + 363 + world_lt_3_offset;
#else
          position.x = D_800757CC + 365 + world_lt_3_offset;
#endif
          position.z = 0x1100;
          sprintf(szBuf, "%d", g_TargetGemCounts[D_80075744 * 6 + i]);
#if GAME_VER == VER_NTSC_U
          func_80017FE4(szBuf, &position, 18, 2);
#else
          func_800181AC(szBuf, &position, nullptr, 18, 11);
#endif

          // level stats - dragon count
          if (g_TargetDragonCounts[D_80075744 * 6 + i] != 0) {
#if GAME_VER == VER_NTSC_U
            position.x = D_800757CC + 445;
#else
            position.x = D_800757CC + 447;
#endif

            sprintf(szBuf, "%d", g_LevelDragonCount[D_80075744 * 6 + i]);
#if GAME_VER == VER_NTSC_U
            func_80017FE4(szBuf, &position, 18, 2);
#else
            func_800181AC(szBuf, &position, nullptr, 18, 11);
#endif
#if GAME_VER == VER_NTSC_U
            position.x = D_800757CC + 462;
#else
            position.x = D_800757CC + 464;
#endif
            position.z = 0x1800;
#if GAME_VER == VER_NTSC_U
            func_80017FE4("/", &position, 18, 2);
#else
            func_800181AC("/", &position, nullptr, 18, 11);
#endif
#if GAME_VER == VER_NTSC_U
            position.x = D_800757CC + 479;
#else
            position.x = D_800757CC + 481;
#endif
            position.z = 0x1100;
            sprintf(szBuf, "%d", g_TargetDragonCounts[D_80075744 * 6 + i]);
#if GAME_VER == VER_NTSC_U
            func_80017FE4(szBuf, &position, 18, 2);
#else
            func_800181AC(szBuf, &position, nullptr, 18, 11);
#endif
          } else {
            // use the "/" to make "-" i guess
#if GAME_VER == VER_NTSC_U
            position.x = D_800757CC + 462;
#else
            position.x = D_800757CC + 464;
#endif
            position.z = 0x1800;
#if GAME_VER == VER_NTSC_U
            func_80017FE4("/", &position, 18, 2);
#else
            func_800181AC("/", &position, nullptr, 18, 11);
#endif
            g_HudMobys->m_Rotation.x = 41;
          }

          // make the current level title wiggle
          if (D_80075744 * 6 + i == g_LevelIndex) {
            int j;
            endMoby--;
            for (j = 0; (int)endMoby >= (int)g_HudMobys; ++j) {
              endMoby->m_Rotation.z =
                  (COSINE_8((D_800758B8 * 8 + j * 12) & 0xFF) * 3) >> 9;
              endMoby--;
            }
          }

          if (g_EggTotal > 0 && D_80075744 < 3) {
            int j;
            for (j = 0; j < g_TargetEggCounts[D_80075744 * 6 + i]; j++) {
#if GAME_VER == VER_NTSC_U
              rc2.x = D_800757CC + 232 + j * 24 +
                      ((g_TargetEggCounts[D_80075744 * 6 + i] == 1) ? 24 : 0);
#else
              rc2.x = D_800757CC + 238 + j * 24 +
                      ((g_TargetEggCounts[D_80075744 * 6 + i] == 1) ? 24 : 0);
#endif
              rc2.y = level_y - 8;
              rc2.w = 24;
              rc2.h = 16;
              if (j >= g_LevelEggCount[D_80075744 * 6 + i]) {
                setXYZ(&position, rc2.x + 12, level_y, 0x1800);
#if GAME_VER == VER_NTSC_U
                func_80017FE4("X", &position, 0, 2);
#else
                func_800181AC("X", &position, nullptr, 0, 11);
#endif
                setXYZ(&position, 32, 32, 32);
                func_8001919C(&rc2, &g_Hud.m_OrbAndEggSprite[1],
                              (ColorInt *)&position);
              } else {
                func_8001919C(
                    &rc2,
                    &g_Hud.m_OrbAndEggSprite[(g_Hud.unk_0x40 + i) % 9 + 1],
                    nullptr);
              }
            }
          }
          level_y += 19;
        }
      }
      func_8001844C(D_800757CC + 112, level_y - 11, D_800757CC + 208,
                    level_y - 11);
#if GAME_VER == VER_NTSC_U
      func_8001844C(D_800757CC + 320, level_y - 11, D_800757CC + 384,
                    level_y - 11);
#else
      func_8001844C(D_800757CC + 324, level_y - 11, D_800757CC + 388,
                    level_y - 11);
#endif
#if GAME_VER == VER_NTSC_U
      func_8001844C(D_800757CC + 430, level_y - 11, D_800757CC + 494,
                    level_y - 11);
#else
      func_8001844C(D_800757CC + 432, level_y - 11, D_800757CC + 496,
                    level_y - 11);
#endif
    }

    g_SonyImage.m_ShadedMobys[0] = nullptr;
    if (g_Gamestate == 2 && !g_IsFlightLevel) {
      func_80019300(); // create collectables
    }

    func_80018880();
    Memset(&g_SonyImage, 0, 0x900);
    func_80022A2C();

    DrawOTag(func_80016784(0));

    DrawSync(0);
    if (D_80075784) {
      VSync(0);
    }

    D_80075950.pre = VSync(-1);

    while (D_80075950.pre - D_80075950.post < 2) {
      VSync(0);
      D_80075950.pre = VSync(-1);
    }

    D_80075950.post = VSync(-1);
    PutDispEnv(&g_CurDB->m_DispEnv);
  }

  ++D_800758B8;
}

/// @brief Gamestate 10
void func_8001C694(void) {
  DB *db;
  RECT rc;
  int i;
  POLY_FT4 *f4;
  LINE_F2 *l2;
  int color;

  D_800758B0 = 0;
  D_800757B0 = g_Buffers.m_LowerPolyBuffer;
  D_80075780 = (char *)g_Buffers.m_LowerPolyBuffer + 0x1C000;

  switch (D_800758B8) {
  case 0:
    db = &g_DB[0];

    if (db == g_CurDB) {
      db = &g_DB[1];
    }

    PutDrawEnv(&db->m_DrawEnv);
    setRECT(&rc, 512 + D_8007568C, 224, 32 - D_8007568C, 1);
    LoadImage(&rc, (u_long *)g_GrayscalePalette);
    DrawSync(0);

    for (i = 0; i < 4; ++i) {
      // build the POLY_FT4
      f4 = D_800757B0;

      // setPolyFT4(f4); (Doesn't match, because they zero out the addr field)
      f4->tag = PRIM_TAG(POLY_FT4);
      f4->code = PRIM_POLY_FT4;

      setRGB0(f4, 0x4C, 0x80, 0x40);
      setXY4(f4, i << 7, 8, f4->x0 + 0x80, f4->y0, f4->x0, f4->y0 + 0xDF,
             f4->x0 + 0x80, f4->y0 + 0xDF);
      setUV4(f4, 0, 0, f4->u0 + 0x80, f4->v0, f4->u0, f4->v0 - 0x21,
             f4->u0 + 0x80, f4->v0 - 0x21);

      f4->clut = getClut(512, 224);
      f4->tpage = getTPage(1, 0, 512, 0) + i;

      // add to OT
      func_800168A0(f4, 0x3FF);
      D_800757B0 = f4 + 1;
    }

    // build LINE_F2
    l2 = (LINE_F2 *)(f4 + 1);

    // setLineF2(l2); (Doesn't match, because they zero out the addr field)
    l2->tag = PRIM_TAG(LINE_F2);
    l2->code = PRIM_LINE_F2;

    setXY2(l2, 0, 231, 512, 231);
    setRGB0(l2, 0, 0, 0);

    // add to OT
    func_800168A0(l2, 0x3FF);
    D_800757B0 = l2 + 1;
    break;
  case 1:
    D_800757D4 = 0; // lerp color target
    D_8007575C = (0x20 - D_8007568C) * 0x80;
    color = ColorLerp(*(int *)&g_Cyclorama.m_BackgroundColor, 0, D_8007575C);

    if (g_CurDB == g_DB) {
      setRGB0(&g_DB[1].m_DrawEnv, ((Color *)&color)->r, ((Color *)&color)->g,
              ((Color *)&color)->b);
      PutDrawEnv(&g_DB[1].m_DrawEnv);
    } else {
      setRGB0(&g_DB[0].m_DrawEnv, ((Color *)&color)->r, ((Color *)&color)->g,
              ((Color *)&color)->b);
      PutDrawEnv(&g_DB[0].m_DrawEnv);
    }

    func_8004F000();
    break;
  }

  Memset(g_SonyImage.u.m_Draw.m_Moby, 0, sizeof(g_SonyImage.u.m_Draw.m_Moby));
  func_80023AC4();
  DrawOTag(func_80016784(0x800));

  DrawSync(0);
  if (D_80075784) {
    VSync(0);
  }

  D_80075950.pre = VSync(-1);

  while (D_80075950.pre - D_80075950.post < 2) {
    VSync(0);
    D_80075950.pre = VSync(-1);
  }

  D_80075950.post = VSync(-1);
  PutDispEnv(&g_CurDB->m_DispEnv);
}

/// @brief Gamestate 4 & 5
void func_8001CA38(void) {
  RECT rc;
  int y;
  if (D_80075940 == 0) {
    if (g_GameOverTicks == 0) {
      func_800521C0(); // Queue render mobys
      func_80019698(); // Creates Mobys, shadows, Spyro, flame and glows and
                       // sparkles
      func_800573C8(); // particles
      func_80050BD0();
      func_8002B9CC(); // Create the environment
      DrawSync(0);
      VSync(0);
      PutDispEnv(&g_CurDB->m_DispEnv);
      PutDrawEnv(&g_CurDB->m_DrawEnv);
      DrawOTag(func_80016784(0x800));
      DrawSync(0);
      VSync(0);

      rc.x = 0;
      rc.y = y = g_CurDB != g_DB ? (256 - 8) : 8;
      rc.w = 512;
      rc.h = 224;
      MoveImage(&rc, 0, 256 - y);
      DrawSync(0);
      g_DB[0].m_DrawEnv.isbg = 0;
      g_DB[1].m_DrawEnv.isbg = 0;
    } else if (g_GameOverTicks <= 15) {
      if (g_GameOverTicks == 1) {
        func_800190D4(2, 16, 16, 16);
      } else {
        func_800190D4(2, 32, 32, 32);
      }
      DrawSync(0);
      VSync(0);
      PutDispEnv(&g_CurDB->m_DispEnv);
      PutDrawEnv(&g_CurDB->m_DrawEnv);
      DrawOTag(func_80016784(0x800));
      if (g_GameOverTicks == 15) {
        g_DB[0].m_DrawEnv.isbg = 1;
        g_DB[1].m_DrawEnv.isbg = 1;
        setRGB0(&g_DB[0].m_DrawEnv, 0, 0, 0);
        setRGB0(&g_DB[1].m_DrawEnv, 0, 0, 0);
        DrawSync(0);
        VSync(0);
#if GAME_VER == VER_NTSC_U
        setRECT(&rc, 0, 0, 512, 480);
        ClearImage(&rc, 0, 0, 0);
#else
        ClearFramebuffers();
#endif
      }
    }
  } else if (D_80075940 == 1) {
    if (g_GameOverTicks >= 60) {
      int i;
      Moby *curMoby;
      Vector3D position;
      Vector3D spacing;
      for (i = 0; i < 8; ++i) {
        int tickOff = g_GameOverTicks - 60 - g_GameOverLetterDisplayTicks[i];
        if (tickOff >= 0) {
          --g_HudMobys;
          Memset(g_HudMobys, 0, sizeof(Moby));
          if (tickOff < 24)
            g_HudMobys->m_Position.z = g_GameOverTextZ[tickOff];
          else
            g_HudMobys->m_Position.z = 0x400;
          g_HudMobys->m_Class =
              (D_8006F304[i] & 0xFF) - 'A' + MOBYCLASS_LETTER_A;
          g_HudMobys->m_Position.x = g_GameOverTextXY[i].x;
          g_HudMobys->m_Position.y = g_GameOverTextXY[i].y;
          g_HudMobys->m_Rotation.z = g_GameOverTextRot[i];
          g_HudMobys->m_DepthOffset = '\x04';
          g_HudMobys->m_SpecularMetalType = '\v';
          g_HudMobys->m_RenderRadius = ' ';
        }
      }

      if (g_LoadStage > 10 && g_GameOverTicks > 420) {
        setXYZ(&spacing, 16, 1, 0x1400);
#if GAME_VER == VER_NTSC_U
        setXYZ(&position, 174, 208, 0x1100);
        func_800181AC("PRESS START", &position, &spacing, 18, 11);
#else
        curMoby = g_HudMobys;
        setXYZ(&position, StringPressStartX, 208, 0x1100);
        func_800181AC(StringPressStart, &position, &spacing, 18, 11);
#endif

#if GAME_VER == VER_NTSC_U
        curMoby = g_HudMobys;
        for (i = 0; i < 10; ++i) {
          (*curMoby++).m_Rotation.z =
              COSINE_8((g_GameOverTicks * 4 + i * 12) & 0xFF) * 3 >> 9;
        }
#else
        curMoby--;
        for (i = 0; (int)g_HudMobys <= (int)curMoby; ++i) {
          curMoby->m_Rotation.z =
              COSINE_8((g_GameOverTicks * 4 + i * 12) & 0xFF) * 3 >> 9;
          curMoby--;
        }
#endif
      }

      g_SonyImage.m_ShadedMobys[0] = nullptr;
      func_80018880();
      Memset(&g_SonyImage.u.m_Draw.m_Moby, 0,
             sizeof(g_SonyImage.u.m_Draw.m_Moby));
      func_80022A2C();
      if (g_GameOverTicks > 180) {
        func_80023AC4();
      }
    }
    g_Camera.m_Rotation.y = g_GameOverSkyboxRotY;
    g_Camera.m_Rotation.z = g_GameOverSkyboxRotZ;
    CameraUpdateMatrices();
    func_8004EBA8(-1, &g_Camera.m_ViewMatrix, &g_Camera.m_ProjectionMatrix);
    g_Camera.m_Rotation.y = g_GameOverRotY;
    g_Camera.m_Rotation.z = g_GameOverRotZ;
    if (g_GameOverTicks < 32) {
      int fade = (31 - g_GameOverTicks) * 8;
      func_800190D4(2, fade, fade, fade);
    }
    DrawSync(0);
    VSync(0);
    PutDispEnv(&g_CurDB->m_DispEnv);
    PutDrawEnv(&g_CurDB->m_DrawEnv);
    DrawOTag(func_80016784(0x800));
  } else {
    if (g_GameOverTicks == 0) {
      rc.x = 0;
      rc.y = y = g_CurDB != g_DB ? (256 - 8) : 8;
      rc.w = 512;
      rc.h = 224;
      MoveImage(&rc, 0, 256 - y);
      DrawSync(0);
      g_DB[0].m_DrawEnv.isbg = 0;
      g_DB[1].m_DrawEnv.isbg = 0;
    } else if (g_GameOverTicks <= 15) {
      if (g_GameOverTicks == 1) {
        func_800190D4(2, 16, 16, 16);
      } else {
        func_800190D4(2, 32, 32, 32);
      }
      DrawSync(0);
      VSync(0);
      PutDispEnv(&g_CurDB->m_DispEnv);
      PutDrawEnv(&g_CurDB->m_DrawEnv);
      DrawOTag(func_80016784(0x800));
      if (g_GameOverTicks == 15) {
        g_DB[0].m_DrawEnv.isbg = 1;
        g_DB[1].m_DrawEnv.isbg = 1;
        setRGB0(&g_DB[0].m_DrawEnv, 0, 0, 0);
        setRGB0(&g_DB[1].m_DrawEnv, 0, 0, 0);
        DrawSync(0);
        VSync(0);
#if GAME_VER == VER_NTSC_U
        setRECT(&rc, 0, 0, 512, 480);
        ClearImage(&rc, 0, 0, 0);
#else
        ClearFramebuffers();
#endif
      }
    }
  }
}

/// @brief Gamestate 8
void func_8001CFDC(void) {
  char _[16];

  if (g_DragonCutsceneFlash.unk_0x0 != 0) {
    RotVec8ToMatrix(&g_DragonCutsceneFlash.vec_0x28,
                    (MATRIX *)&g_DragonCutsceneFlash.mat_0x10,
                    (MATRIX *)&g_Camera.m_ProjectionMatrix);
    func_80058864();
  }

  if (g_DragonCutscene.m_State == 0) {
    func_800521C0(); // Queue render mobys
    func_80019698(); // Creates Mobys, shadows, Spyro, flame and glows and
                     // sparkles
    func_8002B9CC(); // Create the environment
    func_80050BD0(); //
    func_800573C8(); // particles

    if (g_DragonCutscene.m_Fade) {
      // fade in/out
      func_800190D4(1, g_DragonCutscene.m_Fade, g_DragonCutscene.m_Fade,
                    g_DragonCutscene.m_Fade);
    }

    if (g_ScreenBorderEnabled || D_800756C0) {
      func_80018F30(); // create border
    }
  } else if (g_DragonCutscene.m_State < 4) {
    {
      Moby **mobyBuf = g_SonyImage.u.m_Draw.m_Moby;
      if (g_DragonCutscene.m_RescuedDragonMoby->m_State < 0x80)
        *mobyBuf++ = g_DragonCutscene.m_RescuedDragonMoby;
      if (g_DragonCutscene.m_State == 3)
        *mobyBuf++ = g_DragonCutscene.m_CutsceneDragon;
      *mobyBuf = nullptr;
    }

    // copy over the still-visible dragon fragments
    {
      Moby **mobyBuf = g_SonyImage.m_ShadedMobys;
      Moby *lvlMoby;
      for (lvlMoby = g_LevelMobys; lvlMoby->m_State != 0xFF; ++lvlMoby) {
        if (lvlMoby->m_Class == MOBYCLASS_CRYSTAL_DRAGON_FRAGMENT &&
            lvlMoby->m_State < 0x80)
          *mobyBuf++ = lvlMoby;
      }
      *mobyBuf = nullptr;
    }

    func_80018728(); // Create "Rescued ..." text
    func_80018880(); // Copy HUD mobys to shaded mobys
    func_8001F158(); // Prepare Moby draw
    Memset(g_SonyImage.u.m_Draw.m_Moby, 0, sizeof(g_SonyImage.u.m_Draw.m_Moby));
    func_8001F798(); // Draw mobys
    func_80022A2C(); // Render shaded mobys
    func_80059F8C(); // Render moby shadows
    func_80023AC4(); // Render Spyro model
    func_80059A48(); // Render Spyro shadow
    func_8002B9CC(); // Create environment
    func_80050BD0();
    func_800573C8(); // Render particles

    if (g_DragonCutscene.m_Fade) {
      // fade in/out
      func_800190D4(1, g_DragonCutscene.m_Fade, g_DragonCutscene.m_Fade,
                    g_DragonCutscene.m_Fade);
    }

    if (g_ScreenBorderEnabled || D_800756C0) {
      func_80018F30(); // Create border
    }
  } else if (g_DragonCutscene.m_State == 4) {
    {
      Moby **mobyBuf = g_SonyImage.u.m_Draw.m_Moby;
      *mobyBuf++ = g_DragonCutscene.m_CutsceneDragon;
      *mobyBuf++ = g_DragonCutscene.m_CutsceneSpyro;
      *mobyBuf = nullptr;
    }

    {
      Moby **mobyBuf = g_SonyImage.m_ShadedMobys;
      Moby *lvlMoby;
      for (lvlMoby = g_LevelMobys; lvlMoby->m_State != 0xFF; ++lvlMoby) {
        if (lvlMoby->m_Class == MOBYCLASS_CRYSTAL_DRAGON_FRAGMENT &&
            lvlMoby->m_State < 0x80)
          *mobyBuf++ = lvlMoby;
      }
      *mobyBuf = nullptr;
    }

    if (g_DragonCutscene.m_CutsceneTicks < 60) {
      func_80018728(); // Create "Rescued ..." text
      func_80018880(); // Copy HUD mobys to shaded mobys
    }

    func_8001F158(); // Prepare Moby draw
    Memset(g_SonyImage.u.m_Draw.m_Moby, 0, sizeof(g_SonyImage.u.m_Draw.m_Moby));
    func_8001F798(); // Draw mobys
    func_80022A2C(); // Render shaded mobys
    func_80059F8C(); // Render moby shadows
    func_80059A48(); // Render Spyro shadow
    func_8002B9CC(); // Create environment
    func_80050BD0();
    func_800573C8(); // Render particles

    if (g_ScreenBorderEnabled || D_800756C0) {
      func_80018F30(); // Create border
    }
  } else if (g_DragonCutscene.m_State == 5) {
    {
      Moby **mobyBuf = g_SonyImage.u.m_Draw.m_Moby;
      *mobyBuf++ = g_DragonCutscene.m_CutsceneDragon;
      *mobyBuf = nullptr;
    }

    // copy over the hud dragon/counts
    {
      Moby **mobyBuf = g_SonyImage.m_ShadedMobys;
      int i;
      for (i = 0; i < 3; ++i) {
        *mobyBuf++ = g_Hud.m_Mobys + 5 + i;
      }
      *mobyBuf = nullptr;
    }

    func_8001F158(); // Prepare Moby draw
    if (g_SonyImage.u.m_Draw.unk_0x1600 != 0) {
      int value = D_8006F3C0[g_DragonCutscene.m_CutsceneTicks];
      g_SonyImage.u.m_Draw.unk_0x161C.x =
          FIXED_MUL(g_SonyImage.u.m_Draw.unk_0x161C.x, value);
      g_SonyImage.u.m_Draw.unk_0x161C.y =
          FIXED_MUL(g_SonyImage.u.m_Draw.unk_0x161C.y, value);
      g_SonyImage.u.m_Draw.unk_0x161C.z =
          FIXED_MUL(g_SonyImage.u.m_Draw.unk_0x161C.z, value);
      if (g_DragonCutscene.m_CutsceneTicks > 8) {
        value = Cos((g_DragonCutscene.m_CutsceneTicks - 8) * 42);
        g_SonyImage.u.m_Draw.unk_0x1622.x =
            FIXED_MUL(g_SonyImage.u.m_Draw.unk_0x1622.x, value);
        g_SonyImage.u.m_Draw.unk_0x1622.y =
            FIXED_MUL(g_SonyImage.u.m_Draw.unk_0x1622.y, value);
        g_SonyImage.u.m_Draw.unk_0x1622.z =
            FIXED_MUL(g_SonyImage.u.m_Draw.unk_0x1622.z, value);
      }
    }

    Memset(g_SonyImage.u.m_Draw.m_Moby, 0, sizeof(g_SonyImage.u.m_Draw.m_Moby));
    func_8001F798(); // Draw mobys
    func_80022A2C(); // Render shaded mobys
    func_80023AC4(); // Render Spyro model
    func_80059A48(); // Render Spyro shadow
    func_8002B9CC(); // Create environment
    func_80050BD0();
    func_800573C8(); // Render particles

    if (g_ScreenBorderEnabled || D_800756C0) {
      func_80018F30(); // Create border
    }
  } else if (g_DragonCutscene.m_State == 6) {
    int i;
    Moby **mobyBuf = g_SonyImage.m_ShadedMobys;
    for (i = 0; i < 3; ++i) {
      *mobyBuf++ = g_Hud.m_Mobys + 5 + i;
    }
    *mobyBuf = nullptr;

    Memset(g_SonyImage.u.m_Draw.m_Moby, 0, sizeof(g_SonyImage.u.m_Draw.m_Moby));
    func_80022A2C(); // Render shaded mobys
    func_80023AC4(); // Render Spyro model
    func_80059A48(); // Render Spyro shadow
    func_8002B9CC(); // Create environment
    func_80050BD0();
    func_800573C8(); // Render particles

    if (g_ScreenBorderEnabled || D_800756C0) {
      func_80018F30(); // Create border
    }
  } else if (g_DragonCutscene.m_State == 7) {
    int i;
    if (g_DragonCutscene.m_CutsceneTicks < 16) {
      Moby **mobyBuf = g_SonyImage.m_ShadedMobys;
      for (i = 0; i < 3; ++i) {
        *mobyBuf++ = g_Hud.m_Mobys + 5 + i;
      }
      *mobyBuf = nullptr;

      Memset(g_SonyImage.u.m_Draw.m_Moby, 0,
             sizeof(g_SonyImage.u.m_Draw.m_Moby));
      func_80022A2C(); // Render shaded mobys
    } else {
      func_800521C0();
      func_8001F158(); // Prepare Moby draw
      Memset(g_SonyImage.u.m_Draw.m_Moby, 0,
             sizeof(g_SonyImage.u.m_Draw.m_Moby));
      func_8001F798(); // Draw mobys
      func_800208FC(); // Init shiny moby renderer
      func_80020F34();
      func_80022A2C(); // Render shaded mobys
      func_80059F8C(); // Render moby shadows
    }
    func_80023AC4(); // Render Spyro model
    func_80059A48(); // Render Spyro shadow
    func_8002B9CC(); // Create environment
    func_80050BD0();
    func_800573C8(); // Render particles

    if (g_ScreenBorderEnabled || D_800756C0) {
      func_80018F30(); // Create border
    }

    if (g_DragonCutscene.m_Fade) {
      // fade in/out
      func_800190D4(1, g_DragonCutscene.m_Fade, g_DragonCutscene.m_Fade,
                    g_DragonCutscene.m_Fade);
    }
  }

  DrawSync(0);
  if (D_80075784) {
    VSync(0);
  }

  D_80075950.pre = VSync(-1);

  while (D_80075950.pre - D_80075950.post < 2) {
    VSync(0);
    D_80075950.pre = VSync(-1);
  }

  D_80075950.post = VSync(-1);
  PutDispEnv(&g_CurDB->m_DispEnv);
  PutDrawEnv(&g_CurDB->m_DrawEnv);
  DrawOTag(func_80016784(0x800));
}

typedef struct {
  short x, x2;
  short y, y2;
} FAIRYRECT;

#if GAME_VER == VER_NTSC_U
extern FAIRYRECT g_FairyDialogueBoxSizes[8];
#else
extern short g_FairyDialogueBoxSizes[8];
#endif

/// @brief Gamestate 11
void func_8001D718(void) {
  int i;

  Vector3D position;
  Vector3D spacing;

  Moby *curMoby;
  unsigned char textLen;
#if GAME_VER >= VER_C3_DEMO_NTSC_U
  Moby *textStart;
  int boxLeft;
  int boxRight;
#endif

  // Dunno what this is about
  if (D_800756FC == g_Buffers.m_HudOTStart) {
    // Reduces the polygon space available from 0x1C000 to 0x1BA00
    D_800756FC = (char *)D_800757B0 + 0x1BA00;

    // Then uses that space to store the list of HUD Mobys
    g_HudMobys = D_800756FC;
  }

  if (g_FairyCutscene.m_State == 1) {
#if GAME_VER == VER_NTSC_U
    func_8001860C(
        g_FairyCutscene.m_MenuOffsetX +
            g_FairyDialogueBoxSizes[g_FairyCutscene.m_MenuDialoguePage].x,
        g_FairyCutscene.m_MenuOffsetX +
            g_FairyDialogueBoxSizes[g_FairyCutscene.m_MenuDialoguePage].x2,
        g_FairyDialogueBoxSizes[g_FairyCutscene.m_MenuDialoguePage].y,
        g_FairyDialogueBoxSizes[g_FairyCutscene.m_MenuDialoguePage].y2);

    spacing.x = 16;
    spacing.y = 1; // ... Why?
    spacing.z = 0x1400;

    position.z = 0x1100;

    switch (g_FairyCutscene.m_MenuDialoguePage) {
    case 0: // HI SPYRO
      position.x = g_FairyCutscene.m_MenuOffsetX + 64;
      position.y = 50;

      func_800181AC("HI SPYRO,", &position, &spacing, 18, 11);
      position.y += 24;

      position.x = g_FairyCutscene.m_MenuOffsetX + 76;
      func_800181AC("SAVE GAME", &position, &spacing, 18, 11);
      position.y += 19;

      // Selected
      if (g_FairyCutscene.m_MenuSelectedOption == 0) {
        curMoby = g_HudMobys;
        textLen = sizeof("SAVEGAME") - 1;
      }

      position.x = g_FairyCutscene.m_MenuOffsetX + 76;
      func_800181AC("REPLAY DRAGON", &position, &spacing, 18, 11);
      position.y += 19;

      if (g_FairyCutscene.m_MenuSelectedOption == 1) {
        curMoby = g_HudMobys;
        textLen = sizeof("REPLAYDRAGON") - 1;
      }

      position.x = g_FairyCutscene.m_MenuOffsetX + 76;
      func_800181AC("CONTINUE", &position, &spacing, 18, 11);

      if (g_FairyCutscene.m_MenuSelectedOption == 2) {
        curMoby = g_HudMobys;
        textLen = sizeof("CONTINUE") - 1;
      }

      // You're going to be seeing a lot of this
      for (i = 0; i < textLen; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) *
                3 >>
            9;
      }
      break;
    case 1: // NO SAVE FILE
      position.y = 50;
      position.x = g_FairyCutscene.m_MenuOffsetX + 82;
      func_800181AC("NO SAVE FILE", &position, &spacing, 18, 11);

      position.y += 26;

      curMoby = g_HudMobys;

      for (i = 0; i < (int)sizeof("NOSAVEFILE") - 1; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) *
                3 >>
            9;
      }

      spacing.x = 14;
      spacing.y = 1;
      spacing.z = 0x1600;
      position.z = 0x1400;

      position.x = g_FairyCutscene.m_MenuOffsetX + 45;
      func_800181AC("PLEASE RESTART WITH", &position, &spacing, 16, 11);
      position.y += 17;

      position.x = g_FairyCutscene.m_MenuOffsetX + 67;
      func_800181AC("A MEMORY CARD TO", &position, &spacing, 16, 11);
      position.y += 17;

      position.x = g_FairyCutscene.m_MenuOffsetX + 56;
      func_800181AC("ENABLE GAME SAVES.", &position, &spacing, 16, 11);

      break;
    case 2: // SAVING...
      position.y = 82;
      position.x = g_FairyCutscene.m_MenuOffsetX + 106;
      func_800181AC("SAVING...", &position, &spacing, 18, 11);
      curMoby = g_HudMobys;

      for (i = 0; i < (int)sizeof("SAVING...") - 1; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) *
                3 >>
            9;
      }
      break;
    case 3: // NO MEMORY CARD
    case 4: // NO SAVE FILE
      position.y = 50;

      if (g_FairyCutscene.m_MenuDialoguePage == 3) {
        position.x = g_FairyCutscene.m_MenuOffsetX + 66;
        func_800181AC("NO MEMORY CARD", &position, &spacing, 18, 11);
        textLen = sizeof("NOMEMORYCARD") - 1;
      } else {
        position.x = g_FairyCutscene.m_MenuOffsetX + 84;
        func_800181AC("NO SAVE FILE", &position, &spacing, 18, 11);
        textLen = sizeof("NOSAVEFILE") - 1;
      }

      curMoby = g_HudMobys;
      position.y += 26;

      for (i = 0; i < textLen; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) *
                3 >>
            9;
      }

      spacing.x = 14;
      spacing.y = 1;
      spacing.z = 0x1600;

      position.x = g_FairyCutscene.m_MenuOffsetX + 59;
      position.z = 0x1400;

      func_800181AC("PLEASE INSERT THE", &position, &spacing, 16, 11);
      position.y += 17;

      position.x = g_FairyCutscene.m_MenuOffsetX + 67;
      func_800181AC("MEMORY CARD WITH", &position, &spacing, 16, 11);
      position.y += 17;

      position.x = g_FairyCutscene.m_MenuOffsetX + 45;
      func_800181AC("THIS GAME SAVE FILE", &position, &spacing, 16, 11);

      break;
    case 5: // SAVE ERROR
      position.y = 50;
      position.x = g_FairyCutscene.m_MenuOffsetX + 94;
      func_800181AC("SAVE ERROR", &position, &spacing, 18, 11);
      position.y += 26;

      curMoby = g_HudMobys;

      for (i = 0; i < (int)sizeof("SAVEERROR") - 1; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) *
                3 >>
            9;
      }

      spacing.x = 14;
      spacing.y = 1;
      spacing.z = 0x1600;
      position.z = 0x1400;

      position.x = g_FairyCutscene.m_MenuOffsetX + 59;
      func_800181AC("PLEASE CHECK THAT", &position, &spacing, 16, 11);
      position.y += 17;

      position.x = g_FairyCutscene.m_MenuOffsetX + 73;
      func_800181AC("THE MEMORY CARD", &position, &spacing, 16, 11);
      position.y += 17;

      position.x = g_FairyCutscene.m_MenuOffsetX + 54;
      func_800181AC("IS STILL IN PLACE.", &position, &spacing, 16, 11);
      break;
    case 6: // SAVE FAILED
      position.y = 53;
      position.x = g_FairyCutscene.m_MenuOffsetX + 86;
      func_800181AC("SAVE FAILED", &position, &spacing, 18, 11);
      position.y += 40;

      position.x = g_FairyCutscene.m_MenuOffsetX + 135;
      func_800181AC("RETRY", &position, &spacing, 18, 11);
      position.y += 19;

      // Selected
      if (g_FairyCutscene.m_MenuSelectedOption == 0) {
        curMoby = g_HudMobys;
        textLen = sizeof("RETRY") - 1;
      }

      position.x = g_FairyCutscene.m_MenuOffsetX + 135;
      func_800181AC("ABORT", &position, &spacing, 18, 11);

      // Selected
      if (g_FairyCutscene.m_MenuSelectedOption == 1) {
        curMoby = g_HudMobys;
        textLen = sizeof("ABORT") - 1;
      }

      for (i = 0; i < textLen; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) *
                3 >>
            9;
      }

      break;
    case 7: // GAME SAVED
      position.y = 82;
      position.x = g_FairyCutscene.m_MenuOffsetX + 94;
      func_800181AC("GAME SAVED", &position, &spacing, 18, 11);

      curMoby = g_HudMobys;

      for (i = 0; i < (int)sizeof("GAMESAVED") - 1; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) *
                3 >>
            9;
      }
      break;
    }

    if (g_FairyCutscene.m_MenuDialoguePage > 1) {
      // Show the used slot number
      spacing.x = 13;
      spacing.y = 1;
      spacing.z = 0x1a00;

      position.y = 29;
      position.z = 0x1600;

      if (g_FairyCutscene.m_MemoryCardSlot == 0) {
        position.x = g_FairyCutscene.m_MenuOffsetX + 48;
        func_800181AC("SLOT 1", &position, &spacing, 14, 11);
      } else {
        position.x = 230; // SKELETON: Not adding the x offset now?
        func_800181AC("SLOT 2", &position, &spacing, 14, 11);
      }
    }
#else
    if (g_FairyCutscene.m_MenuOffsetX != 0) {
      boxRight = 492;
      boxLeft = boxRight - g_FairyDialogueBoxSizes[g_FairyCutscene.m_MenuDialoguePage];
    } else {
      boxLeft = 20;
      boxRight = g_FairyDialogueBoxSizes[g_FairyCutscene.m_MenuDialoguePage] + 20;
    }
    func_8001860C(boxLeft, boxRight, 36, 124);

    spacing.x = 14;
    spacing.y = 1;
    spacing.z = 0x1600;
    position.z = 0x1400;

    switch (g_FairyCutscene.m_MenuDialoguePage) {
    case 0:
      position.x = boxLeft + 20;
      position.y = 50;
      position.z = 0x1100;
      spacing.x = 16;
      spacing.y = 1;
      spacing.z = 0x1400;
      func_800181AC(StringHiSpyro, &position, &spacing, 18, 11);

      spacing.x = 14;
      spacing.y = 1;
      spacing.z = 0x1600;
      position.z = 0x1400;
      textStart = g_HudMobys;
      position.x = boxLeft + 28;
      position.y += 24;
      func_800181AC(StringSaveGame, &position, &spacing, 16, 11);
      position.y += 19;
      if (g_FairyCutscene.m_MenuSelectedOption == 0) {
        curMoby = g_HudMobys;
        textLen = textStart - curMoby;
      }

      textStart = g_HudMobys;
      position.x = boxLeft + 28;
      func_800181AC(StringReplayDragon, &position, &spacing, 16, 11);
      position.y += 19;
      if (g_FairyCutscene.m_MenuSelectedOption == 1) {
        curMoby = g_HudMobys;
        textLen = textStart - curMoby;
      }

      textStart = g_HudMobys;
      position.x = boxLeft + 28;
      func_800181AC(StringContinue_Fairy, &position,
                    &spacing, 16, 11);
      if (g_FairyCutscene.m_MenuSelectedOption == 2) {
        curMoby = g_HudMobys;
        textLen = textStart - curMoby;
      }
      for (i = 0; i < textLen; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) * 3 >> 9;
      }
      break;

    case 1:
      textStart = g_HudMobys;
      position.y = 51;
      position.x = boxLeft + StringNoSaveFileXOffset;
      func_800181AC(StringNoSaveFile, &position, &spacing, 16, 11);
      curMoby = g_HudMobys;
      textLen = textStart - curMoby;
      position.y += 24;
      for (i = 0; i < textLen; i++) {
        (curMoby++)->m_Rotation.z =
            COSINE_8(((g_FairyCutscene.m_AnimationTimer * 4) + (i * 12)) &
                     0xFF) * 3 >> 9;
      }

      position.x = boxLeft + StringPleaseRestartWithXOffset;
      func_800181AC(StringPleaseRestartWith, &position, &spacing, 16, 11);
      position.y += 18;
      position.x = boxLeft + StringAMemoryCardToXOffset;
      func_800181AC(StringAMemoryCardTo, &position, &spacing, 16, 11);
      position.y += 18;
      position.x = boxLeft + StringEnableSaveGamesXOffset;
      func_800181AC(StringEnableSaveGames, &position, &spacing, 16, 11);
      break;
    }
#endif
  }

  func_800521C0();
  func_80019698();

  if (g_FairyCutscene.m_State == 1) {
    g_SonyImage.m_ShadedMobys[0] = nullptr;
    func_80018880(); // Copy HUD Mobys to Shaded
    func_80022A2C(); // Draw Shaded Mobys
  }

  func_8002B9CC();
  func_80050BD0();
  func_800573C8();
  func_80018F30();

  DrawSync(0);

  if (D_80075784) {
    VSync(0);
  }

  D_80075950.pre = VSync(-1);

  while (D_80075950.pre - D_80075950.post < 2) {
    VSync(0);
    D_80075950.pre = VSync(-1);
  }

  D_80075950.post = VSync(-1);

  PutDispEnv(&g_CurDB->m_DispEnv);
  PutDrawEnv(&g_CurDB->m_DrawEnv);
  DrawOTag(func_80016784(0x800));
}

/// @brief Gamestate 12, balloon transition and balloonist talk
void func_8001E24C(void) {
  Moby *curMoby;
  int rotOff;
  int color;

  Vector3D position;
  Vector3D spacing;
  char buf[64];

  if (g_Balloonist.m_State >= 4) {

    if (g_Balloonist.m_State < 6) {
      Moby **mobyQueue = (void *)g_SonyImage.u.m_Buf;

      *(mobyQueue++) = g_Balloonist.m_BalloonMoby;
      *(mobyQueue++) = (void *)0;

      if (g_Balloonist.m_Homeworld < 5) {
        // Minus one is due to the homeworlds starting at 10
        int homeworld = g_NextLevelId / 10;
#if GAME_VER == VER_NTSC_U
        sprintf(buf, "ENTERING %s WORLD", g_HomeworldNames[homeworld - 1]);
#else
        sprintf(buf, "%s %s %s",
                StringEntering_World,
                g_HomeworldNames[homeworld - 1], StringWorld);
#endif
      } else if (g_Balloonist.m_Homeworld == 5) {
        // Exception for Gnasty's World so it doesn't display
        // GNASTY'S WORLD WORLD (like in July)
#if GAME_VER == VER_NTSC_U
        int homeworld = g_NextLevelId / 10;
        sprintf(buf, "ENTERING %s", g_HomeworldNames[homeworld - 1]);
#else
        sprintf(buf, "%s",
                StringEnteringGnastysWorld);
#endif
      } else {
        int homeworld = g_NextLevelId / 10;
        int level = g_NextLevelId % 10;
#if GAME_VER == VER_NTSC_U
        sprintf(buf, "ENTERING %s",
                g_LevelNames[level + ((homeworld - 1) * 6)]);
#else
        sprintf(buf, "%s %s",
                StringEntering_Level,
                g_LevelNames[level + ((homeworld - 1) * 6)]);
#endif
      }

      // sprintf(buf, fmt, levelName);
      position.x = 256 - ((strlen(buf) - 1) * 7);
      position.y = 200;
      position.z = 0x1400;

      spacing.x = 14;
      spacing.y = 1;
      spacing.z = 0x1600;

      curMoby = g_HudMobys;

#if GAME_VER == VER_NTSC_U
      func_800181AC(buf, &position, &spacing, 16, 2);
#else
      func_800181AC(buf, &position, &spacing, 16, 11);
#endif

      rotOff = 0;
      while ((int)(--curMoby) >= (int)g_HudMobys) {
        curMoby->m_Rotation.z =
            COSINE_8(((g_Balloonist.unk_0x04 * 2) + rotOff) & 0xFF) >> 7;
        rotOff += 12;
      }

      func_8001F158(); // Prepare Moby draw
      Memset(g_SonyImage.u.m_Buf, 0, 0x900);
      func_8001F798(); // Draw Mobys

      g_SonyImage.m_ShadedMobys[0] = nullptr;
      func_80018880(); // Copy HUD Mobys to Shaded
      func_80022A2C(); // Draw Shaded Mobys
      func_80023AC4(); // Draw Spyro

      if (g_Balloonist.m_State < 6) {
        D_800757D4 = 0x401010;

        if (g_Balloonist.m_State == 4) {
          D_8007575C = g_Balloonist.unk_0x04 * 16;
        } else {
          D_8007575C = (0x100 - g_Balloonist.unk_0x04) * 16;
        }

        if (D_8007575C < 0)
          D_8007575C = 0;
        if (D_8007575C > 4096)
          D_8007575C = 4096;

        color = ColorLerp(*(int *)&g_Cyclorama.m_BackgroundColor, D_800757D4,
                          D_8007575C);

        setRGB0(&g_DB[0].m_DrawEnv, ((Color *)&color)->r, ((Color *)&color)->g,
                ((Color *)&color)->b);
        setRGB0(&g_DB[1].m_DrawEnv, ((Color *)&color)->r, ((Color *)&color)->g,
                ((Color *)&color)->b);

        if (D_8007575C < 4096) {
          func_8004F000();
        }
      }
    } else {
      g_BalloonistDraw();
    }
  } else {
    g_BalloonistDraw();
  }

  func_80018F30();

  DrawSync(0);

  if (D_80075784) {
    VSync(0);
  }

  D_80075950.pre = VSync(-1);

  while (D_80075950.pre - D_80075950.post < 2) {
    VSync(0);
    D_80075950.pre = VSync(-1);
  }

  D_80075950.post = VSync(-1);

  PutDispEnv(&g_CurDB->m_DispEnv);
  PutDrawEnv(&g_CurDB->m_DrawEnv);
  DrawOTag(func_80016784(0x800));
}

/// @brief Gamestate 13
void func_8001E6B8(void) {
#if GAME_VER == VER_NTSC_U
  const int textStartTick = 140;
#else
  const int textStartTick = 132;
#endif
  if (g_TitlescreenState.m_State == TSS_Active &&
      g_TitlescreenState.m_Tick >= textStartTick) {
    int a, i;
    Moby *curMoby;

    Vector3D position;
    Vector3D spacing;

    position.y = 120;
    position.z = 5120;

    spacing.x = 14;
    spacing.y = 1;
    spacing.z = 5632;

    curMoby = g_HudMobys;

#if GAME_VER == VER_NTSC_U
    if (g_TitlescreenState.m_DemoType == TSD_Cutscene) {
      position.x = 92;
      func_800181AC("IN THE WORLD OF DRAGONS...", &position, &spacing, 0x10,
                    0xB);
      a = 184;
    } else if (g_TitlescreenState.m_DemoType == TSD_Level) {

      // Have we visited Artisans before?
      if (g_VisitedFlags[0]) {
        position.x = 80;
        func_800181AC("THE ADVENTURE CONTINUES...", &position, &spacing, 0x10,
                      0xB);
        a = 188;
      } else {
        position.x = 100;
        func_800181AC("THE ADVENTURE BEGINS...", &position, &spacing, 0x10,
                      0xB);
        a = 182;
      }

    } else {
      position.x = 104;
      func_800181AC("ENTERING DEMO MODE...", &position, &spacing, 0x10, 0xB);
      a = 178;
    }

    // Hide letters based on the number of ticks ran
    if (a > g_TitlescreenState.m_Tick) {
      g_HudMobys += (a - g_TitlescreenState.m_Tick) >> 1;
    }
#else
    position.x = StringTheAdventureBeginsX;
    func_800181AC(StringTheAdventureBegins, &position, &spacing, 0x10, 0xB);
    a = StringTheAdventureBeginsRevealTick;
    if (g_TitlescreenState.m_Tick > a) {
      if (curMoby - g_HudMobys > (g_TitlescreenState.m_Tick - a) >> 1) {
        g_HudMobys = curMoby - ((g_TitlescreenState.m_Tick - a) >> 1);
      }
    } else {
      g_HudMobys = curMoby;
    }
#endif

    i = 0;
    while ((int)(--curMoby) >= (int)g_HudMobys) {
#if GAME_VER == VER_NTSC_U
      int l = g_TitlescreenState.m_Tick - 140 - i;
#else
      int letterTick = i + 140;
      int l = g_TitlescreenState.m_Tick - letterTick;
#endif

      if (l < 56) {
        curMoby->m_Rotation.z = l * 8 + 64;
      } else {
        curMoby->m_Rotation.z =
            COSINE_8(((g_TitlescreenState.m_Tick * 4) + (i * 12)) & 0xFF) >> 7;
      }

      i++;
    }
  }

  setRGB0(&g_DB[0].m_DrawEnv, 0, 0, 0);
  setRGB0(&g_DB[1].m_DrawEnv, 0, 0, 0);

  g_SonyImage.m_ShadedMobys[0] = nullptr;
  func_80018880(); // Copies the HUD Mobys to the Shaded Mobys list
  Memset(g_SonyImage.u.m_Buf, 0, 0x900);
  func_80022A2C(); // Renders the shaded mobys

  if (g_TitlescreenState.m_State == TSS_Active) {
    // Draw Spyro
    func_80023AC4();

    // Put the 3D text inside of the world
#if GAME_VER == VER_NTSC_U
    ((int *)g_WorldOT)[64] = ((int *)g_HudOT)[0];
    ((int *)g_WorldOT)[65] = ((int *)g_HudOT)[1];
#else
    ((int *)g_WorldOT)[128] = ((int *)g_HudOT)[0];
    ((int *)g_WorldOT)[129] = ((int *)g_HudOT)[1];
#endif
    ((int *)g_HudOT)[0] = 0;
    ((int *)g_HudOT)[1] = 0;
  }

  DrawSync(0);
  VSync(0);
  PutDispEnv(&g_CurDB->m_DispEnv);
  PutDrawEnv(&g_CurDB->m_DrawEnv);
  DrawOTag(func_80016784(0x800));
}

/// @brief Gamestate 14
#if GAME_VER == VER_NTSC_U
void func_8001E9C8(void) {

  setRGB0(&g_DB[0].m_DrawEnv, g_Cyclorama.m_BackgroundColor.r,
          g_Cyclorama.m_BackgroundColor.g, g_Cyclorama.m_BackgroundColor.b);
  setRGB0(&g_DB[1].m_DrawEnv, g_Cyclorama.m_BackgroundColor.r,
          g_Cyclorama.m_BackgroundColor.g, g_Cyclorama.m_BackgroundColor.b);

  func_800521C0();
  func_8001F158();
  Memset(g_SonyImage.u.m_Buf, 0, 0x900);
  func_8001F798();
  Memset(g_SonyImage.u.m_Buf, 0, 0x1C00);

  g_Environment.m_CullingDistance = 0x14000;

  func_800258F0(-1);
  func_8004EBA8(-1, &g_Camera.m_ViewMatrix, &g_Camera.m_ProjectionMatrix);

  if (g_Fade) {
    func_800190D4(2, g_Fade * 16, g_Fade * 16, g_Fade * 16);
  }

  DrawSync(0);

  // No clue why this is here, but it is
  if (D_80075784) {
    VSync(0);
  }

  D_80075950.pre = VSync(-1);

  while (D_80075950.pre - D_80075950.post < 2) {
    VSync(0);
    D_80075950.pre = VSync(-1);
  }

  D_80075950.post = VSync(-1);

  PutDispEnv(&g_CurDB->m_DispEnv);
  PutDrawEnv(&g_CurDB->m_DrawEnv);
  DrawOTag(func_80016784(0x800));
}
#endif

/// @brief Gamestate 15
#if GAME_VER == VER_NTSC_U
void func_8001EB80(void) {
  Vector3D spacing;
  Vector3D position;
  Moby *curMoby;
  int rotOff;

  spacing.x = 16;
  spacing.y = 1;
  spacing.z = 5120;

  position.x = 118;
  position.y = 110;
  position.z = 4352;

  curMoby = g_HudMobys;

  setRGB0(&g_DB[0].m_DrawEnv, 0, 0, 0);
  setRGB0(&g_DB[1].m_DrawEnv, 0, 0, 0);

  func_800181AC("RETURNING HOME...", &position, &spacing, 18, 11);

  rotOff = 0;

  // Wtf are these int casts?
  while ((int)(--curMoby) >= (int)g_HudMobys) {
    curMoby->m_Rotation.z =
        COSINE_8(((g_CreditsTimer * 2) + rotOff) & 0xFF) >> 7;
    rotOff += 12;
  };

  g_SonyImage.m_ShadedMobys[0] = (void *)0;

  func_80018880(); // Copies the HUD Mobys to the Shaded Mobys list
  Memset(g_SonyImage.u.m_Buf, 0, 0x900);
  func_80022A2C(); // Renders the shaded mobys
  DrawSync(0);

  D_80075950.pre = VSync(-1);

  while (D_80075950.pre - D_80075950.post < 2) {
    VSync(0);
    D_80075950.pre = VSync(-1);
  }

  D_80075950.post = VSync(-1);

  PutDispEnv(&g_CurDB->m_DispEnv);
  PutDrawEnv(&g_CurDB->m_DrawEnv);
  DrawOTag(func_80016784(0x800));
}
#endif

void func_credits_8007BFD0(void); // Overlay functions

/// @brief Gamestate draw function
void GamestateDraw(void) {
  DB *db;

  db = &g_DB[0];

  if (db == g_CurDB) {
    db = &g_DB[1];
  }

  g_CurDB = db;
  D_800758B0 = 0;
  g_WorldOT = db->m_WorldOT;
  g_HudOT = db->m_HudOT;
  D_800757B0 = db->m_PolyBuf;
  D_80075780 = (void *)((char *)D_800757B0 + 0x1C000);
  D_800756FC = (void *)((char *)D_800757B0 + 0x1C000);
  g_HudMobys = (void *)((char *)D_800757B0 + 0x1C000);

  CameraUpdateMatrices();

  if (g_Gamestate != GS_Playing) {
    if (g_Gamestate == GS_LevelTransition) {
      func_8001A050();
    } else if (g_Gamestate == GS_PauseMenu) {
      func_8001A40C();
    } else if (g_Gamestate == GS_InventoryMenu) {
      func_8001A40C();
    } else if (g_Gamestate == GS_Respawn) {
      func_8001CA38();
    } else if (g_Gamestate == GS_GameOver) {
      func_8001CA38();
    } else if (g_Gamestate == GS_OldDragon) {
      func_8001A40C();
    } else if (g_Gamestate == GS_FlightResults) {
      // Calls an overlay function
      D_8007567C();
    } else if (g_Gamestate == GS_Dragon) {
      func_8001CFDC();
    } else if (g_Gamestate == GS_EntranceAnimation) {
      func_8001A050();
    } else if (g_Gamestate == GS_ExitLevel) {
      func_8001C694();
    } else if (g_Gamestate == GS_Fairy) {
      func_8001D718();
    } else if (g_Gamestate == GS_Balloonist) {
      func_8001E24C();
    }
#if GAME_VER == VER_NTSC_U
    else if (g_Gamestate == GS_TitleScreen) {
      if (g_TitlescreenState.m_Mode == TSM_Demo) {
        func_8001E6B8();
      } else {
        TitlescreenDraw();
      }
    } else if (g_Gamestate == GS_Cutscene) {
      func_8001E9C8();
    } else if (g_Gamestate == GS_Credits) {
      if (g_CreditsStage < 99) {
        func_credits_8007BFD0(); // Overlay function
      } else {
        func_8001EB80();
      }
    }
#else
    else if (g_Gamestate == GS_TitleScreen) {
      func_8001E6B8();
    }
#endif
  } else {

    // Gamestate 0
    setRGB0(&g_DB[0].m_DrawEnv, g_Cyclorama.m_BackgroundColor.r,
            g_Cyclorama.m_BackgroundColor.g, g_Cyclorama.m_BackgroundColor.b);
    setRGB0(&g_DB[1].m_DrawEnv, g_Cyclorama.m_BackgroundColor.r,
            g_Cyclorama.m_BackgroundColor.g, g_Cyclorama.m_BackgroundColor.b);

    func_800521C0(); // Queue render mobys

    if (!g_IsFlightLevel) {
      func_80019300(); // Draw collectables
    }

    // Is demo mode?
    if (g_DemoMode != DEMO_MODE_NONE) {
      func_80018908(); // Draw demo mode text
    }

    func_80019698();
    func_8002B9CC();
    func_80050BD0();
    func_800573C8();

    if (g_Fade) {
      func_800190D4(2, g_Fade * 8, g_Fade * 8, g_Fade * 8);
    }

    if (g_ScreenBorderEnabled || D_800756C0) {
      func_80018F30();
    }

#if GAME_VER == VER_NTSC_U
    func_800189F0();
#endif
    DrawSync(0);

    // If demo mode, we always wait one frame, probably for demo consistency
    if (D_80075784) {
      VSync(0);
    }

    D_80075950.pre = VSync(-1);

    while (D_80075950.pre - D_80075950.post < 2) {
      VSync(0);
      D_80075950.pre = VSync(-1);
    }

    D_80075950.post = VSync(-1);

    PutDispEnv(&g_CurDB->m_DispEnv);
    PutDrawEnv(&g_CurDB->m_DrawEnv);
    DrawOTag(func_80016784(0x800));
  }
}
