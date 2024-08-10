#include "patch.hpp"

#include "settings/menus.hpp"
#include "cosmetics.hpp"

#include <array>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "../code/src/custom_models.h"

const PatchSymbols UsaSymbols = { GSETTINGSCONTEXT_USA_ADDR,        GSPOILERDATA_USA_ADDR,
                                  GSPOILERDATALOCS_USA_ADDR,        NUMCUSTOMMESSAGEENTRIES_USA_ADDR,
                                  PTRCUSTOMMESSAGEENTRIES_USA_ADDR, RBGMOVERRIDES_USA_ADDR,
                                  RCUSTOMMESSAGES_USA_ADDR,         RDUNGEONINFODATA_USA_ADDR,
                                  RDUNGEONREWARDOVERRIDES_USA_ADDR, RENTRANCEOVERRIDES_USA_ADDR,
                                  RGSLOCOVERRIDES_USA_ADDR,         RITEMOVERRIDES_USA_ADDR,
                                  RSCRUBRANDOMITEMPRICES_USA_ADDR,  RSFXDATA_USA_ADDR,
                                  RSHOPSANITYPRICES_USA_ADDR };

const PatchSymbols EurSymbols = { GSETTINGSCONTEXT_EUR_ADDR,        GSPOILERDATA_EUR_ADDR,
                                  GSPOILERDATALOCS_EUR_ADDR,        NUMCUSTOMMESSAGEENTRIES_EUR_ADDR,
                                  PTRCUSTOMMESSAGEENTRIES_EUR_ADDR, RBGMOVERRIDES_EUR_ADDR,
                                  RCUSTOMMESSAGES_EUR_ADDR,         RDUNGEONINFODATA_EUR_ADDR,
                                  RDUNGEONREWARDOVERRIDES_EUR_ADDR, RENTRANCEOVERRIDES_EUR_ADDR,
                                  RGSLOCOVERRIDES_EUR_ADDR,         RITEMOVERRIDES_EUR_ADDR,
                                  RSCRUBRANDOMITEMPRICES_EUR_ADDR,  RSFXDATA_EUR_ADDR,
                                  RSHOPSANITYPRICES_EUR_ADDR };

// For specification on the IPS file format, visit: https://zerosoft.zophar.net/ips.php

using FILEPtr = std::unique_ptr<FILE, decltype(&std::fclose)>;

bool CopyFile(FS_Archive sdmcArchive, const char* dst, const char* src) {
    Result res = 0;
    Handle outFile;
    u32 bytesWritten = 0;
    // Delete dst if it exists
    FSUSER_DeleteFile(sdmcArchive, fsMakePath(PATH_ASCII, dst));

    // Open dst destination
    if (!R_SUCCEEDED(res = FSUSER_OpenFile(&outFile, sdmcArchive, fsMakePath(PATH_ASCII, dst),
                                           FS_OPEN_WRITE | FS_OPEN_CREATE, 0))) {
        return false;
    }

    if (auto file = FILEPtr{ std::fopen(src, "r"), std::fclose }) {
        // obtain file size
        fseek(file.get(), 0, SEEK_END);
        const auto lSize = static_cast<size_t>(ftell(file.get()));
        rewind(file.get());

        // copy file into the buffer
        std::vector<char> buffer(lSize);
        fread(buffer.data(), 1, buffer.size(), file.get());

        // Write the buffer to final destination
        if (!R_SUCCEEDED(res = FSFILE_Write(outFile, &bytesWritten, 0, buffer.data(), buffer.size(), FS_WRITE_FLUSH))) {
            return false;
        }
    }

    FSFILE_Close(outFile);
    return true;
}

bool WritePatch(u32 patchOffset, s32 patchSize, char* patchDataPtr, Handle& code, u32& bytesWritten, u32& totalRW,
                char* buf) {

    // patch sizes greater than PATCH_SIZE_MAX have to be split up due to IPS patching specifications
    while (patchSize > 0) {
        // Write patch offset address to code
        buf[0] = (patchOffset >> 16) & 0xFF;
        buf[1] = (patchOffset >> 8) & 0xFF;
        buf[2] = (patchOffset) & 0xFF;
        if (!R_SUCCEEDED(FSFILE_Write(code, &bytesWritten, totalRW, buf, 3, FS_WRITE_FLUSH))) {
            return false;
        }
        totalRW += 3;

        // Write patch size to code
        u32 newPatchSize = (patchSize > PATCH_SIZE_MAX) ? PATCH_SIZE_MAX : patchSize;
        buf[0]           = (newPatchSize >> 8) & 0xFF;
        buf[1]           = (newPatchSize) & 0xFF;
        if (!R_SUCCEEDED(FSFILE_Write(code, &bytesWritten, totalRW, buf, 2, FS_WRITE_FLUSH))) {
            return false;
        }
        totalRW += 2;

        // Write patch data to code
        if (!R_SUCCEEDED(FSFILE_Write(code, &bytesWritten, totalRW, patchDataPtr, newPatchSize, FS_WRITE_FLUSH))) {
            return false;
        }
        totalRW += newPatchSize;

        patchDataPtr += PATCH_SIZE_MAX;
        patchOffset += PATCH_SIZE_MAX;
        patchSize -= PATCH_SIZE_MAX;
    }
#ifdef ENABLE_DEBUG
    CitraPrint("patch.cpp: totalRW=" + std::to_string(totalRW));
#endif
    return true;
}

void WriteFloatToBuffer(std::vector<char>& buffer, float f, size_t offset) {
    memcpy(buffer.data() + offset, &f, sizeof(float));
}

bool WriteAllPatches() {
    Result res             = 0;
    FS_Archive sdmcArchive = 0;
    Handle code;
    u32 bytesWritten = 0;
    u32 totalRW      = 0;
    char buf[512];
    
    std::string titleId = Settings::TitleId();
    PatchSymbols patchSymbols;
    
    if (Settings::Region == REGION_EUR) {
        patchSymbols = EurSymbols;
    } else { // REGION_NA
        patchSymbols = UsaSymbols;
    }

    // Open SD archive
    if (!R_SUCCEEDED(res = FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, "")))) {
        return false;
    }

    // Create necessary folders where patch files need to go (if they don't exist);
    // Create the luma directory if it doesn't exist (on citra)
    FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/luma"), FS_ATTRIBUTE_DIRECTORY);
    // Create the titles directory if it doesn't exist
    FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/luma/titles"), FS_ATTRIBUTE_DIRECTORY);
    // Create the 0004000000033500 (33600 for EUR) directory if it doesn't exist (oot3d game id)
    FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, ("/luma/titles/" + titleId).c_str()),
                           FS_ATTRIBUTE_DIRECTORY);
    // Create the romfs directory if it doesn't exist (for LayeredFS)
    FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, ("/luma/titles/" + titleId + "/romfs").c_str()),
                           FS_ATTRIBUTE_DIRECTORY);
    // Create the actor directory if it doesn't exist
    FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, ("/luma/titles/" + titleId + "/romfs/actor").c_str()),
                           FS_ATTRIBUTE_DIRECTORY);

    /*romfs is used to get files from the romfs folder. This allows us to copy
    from basecode and write the exheader without the user needing to worry about
    placing them manually on their SD card.*/
    Result rc = romfsInit();
    if (rc) {
        printf("\nromfsInit: %08lX\n", rc);
    }

    /*-------------------------
    |       basecode.ips      |
    --------------------------*/

    // Delete code.ips if it exists
    FSUSER_DeleteFile(sdmcArchive, fsMakePath(PATH_ASCII, ("/luma/titles/" + titleId + "/code.ips").c_str()));

    // Open code.ips
    if (!R_SUCCEEDED(res = FSUSER_OpenFile(&code, sdmcArchive,
                                           fsMakePath(PATH_ASCII, ("/luma/titles/" + titleId + "/code.ips").c_str()),
                                           FS_OPEN_WRITE | FS_OPEN_CREATE, 0))) {
        return false;
    }

    // Copy basecode to code
    const char* basecodeFile = Settings::Region == REGION_NA ? "romfs:/basecode_USA.ips" : "romfs:/basecode_EUR.ips";
    if (auto basecode = FILEPtr{ std::fopen(basecodeFile, "r"), std::fclose }) {
        // obtain basecode.ips file size
        fseek(basecode.get(), 0, SEEK_END);
        const auto lSize = static_cast<size_t>(ftell(basecode.get()));
        rewind(basecode.get());

        // copy basecode.ips into the buffer
        std::vector<char> buffer(lSize);
        fread(buffer.data(), 1, buffer.size(), basecode.get());

        // Write the buffer to code.ips
        if (!R_SUCCEEDED(
                res = FSFILE_Write(code, &bytesWritten, totalRW, buffer.data(), buffer.size(), FS_WRITE_FLUSH))) {
            return false;
        }

        totalRW += bytesWritten - 3; // -3 to overwrite EOF
#ifdef ENABLE_DEBUG
        CitraPrint("patch.cpp: totalRW=" + std::to_string(totalRW));
#endif
    }

    u32 patchOffset;
    u32 patchSize;

    /*-------------------------
    |     gSettingsContext    |
    --------------------------*/

    patchOffset = V_TO_P(patchSymbols.GSETTINGSCONTEXT_ADDR);
    patchSize   = sizeof(SettingsContext);

    // get the settings context
    SettingsContext ctx = Settings::FillContext();

    if (!WritePatch(patchOffset, patchSize, (char*)(&ctx), code, bytesWritten, totalRW, buf)) {
        return false;
    }

    /*-------------------------
    |           EOF           |
    --------------------------*/

    // Write EOF to code
    if (!R_SUCCEEDED(res =
                         FSFILE_Write(code, &bytesWritten, totalRW, "EOF", 3, FS_WRITE_FLUSH | FS_WRITE_UPDATE_TIME))) {
        return false;
    }

    FSFILE_Close(code);

    /*-------------------------
    |       exheader.bin      |
    --------------------------*/
    // Get exheader for proper playOption
    const char* filePath;
    if (Settings::PlayOption == PATCH_CONSOLE) {
        filePath = "romfs:/exheader.bin";
    } else {
        filePath = "romfs:/exheader_citra.bin";
    }

    CopyFile(sdmcArchive, ("/luma/titles/" + titleId + "/exheader.bin").c_str(), filePath);

    /*-------------------------
    |       custom assets      |
    --------------------------*/

    Cosmetics::Color_RGBAf childTunicColor  = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalChildTunicColor);
    Cosmetics::Color_RGBAf kokiriTunicColor = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalKokiriTunicColor);
    Cosmetics::Color_RGBAf goronTunicColor  = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalGoronTunicColor);
    Cosmetics::Color_RGBAf zoraTunicColor   = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalZoraTunicColor);

    /* Player */
    Cosmetics::Color_RGBAf childTunicColor_1  = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalChildTunicColor_1);
    Cosmetics::Color_RGBAf kokiriTunicColor_1 = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalKokiriTunicColor_1);
    Cosmetics::Color_RGBAf goronTunicColor_1  = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalGoronTunicColor_1);
    Cosmetics::Color_RGBAf zoraTunicColor_1   = Cosmetics::HexStrToColorRGBAf(PersonalizationMenu::finalZoraTunicColor_1);

    // Delete assets if it exists
    Handle assetsOut;
    std::string assetsOutPath = "/luma/titles/" + titleId + "/romfs/actor/zelda_gi_melody.zar";
    const char* assetsInPath  = "romfs:/zelda_gi_melody.zar";
    FSUSER_DeleteFile(sdmcArchive, fsMakePath(PATH_ASCII, assetsOutPath.c_str()));

    // Open assets destination
    if (!R_SUCCEEDED(res = FSUSER_OpenFile(&assetsOut, sdmcArchive, fsMakePath(PATH_ASCII, assetsOutPath.c_str()),
                                           FS_OPEN_WRITE | FS_OPEN_CREATE, 0))) {
        return false;
    }

    if (auto file = FILEPtr{ std::fopen(assetsInPath, "r"), std::fclose }) {
        // obtain assets size
        fseek(file.get(), 0, SEEK_END);
        const auto lSize = static_cast<size_t>(ftell(file.get()));
        rewind(file.get());

        // copy assets into the buffer
        std::vector<char> buffer(lSize);
        fread(buffer.data(), 1, buffer.size(), file.get());

        // obtain offsets for tunic textures
        size_t dataSectionOffsetInZAR = *(size_t*)(buffer.data() + 0x14);
        size_t adultTunicOffsetInZAR  = *(size_t*)(buffer.data() + dataSectionOffsetInZAR + TEXANIM_LINK_BODY * 4);
        size_t childTunicOffsetInZAR = *(size_t*)(buffer.data() + dataSectionOffsetInZAR + TEXANIM_CHILD_LINK_BODY * 4);

        size_t adultTunicOffsetInZAR_1  = *(size_t*)(buffer.data() + dataSectionOffsetInZAR + TEXANIM_LINK_BODY_1 * 4);
        size_t childTunicOffsetInZAR_1 = *(size_t*)(buffer.data() + dataSectionOffsetInZAR + TEXANIM_CHILD_LINK_BODY_1 * 4);

        // edit assets as needed
        WriteFloatToBuffer(buffer, kokiriTunicColor.r, adultTunicOffsetInZAR + 0x70);
        WriteFloatToBuffer(buffer, kokiriTunicColor.g, adultTunicOffsetInZAR + 0x98);
        WriteFloatToBuffer(buffer, kokiriTunicColor.b, adultTunicOffsetInZAR + 0xC0);

        WriteFloatToBuffer(buffer, goronTunicColor.r, adultTunicOffsetInZAR + 0x78);
        WriteFloatToBuffer(buffer, goronTunicColor.g, adultTunicOffsetInZAR + 0xA0);
        WriteFloatToBuffer(buffer, goronTunicColor.b, adultTunicOffsetInZAR + 0xC8);

        WriteFloatToBuffer(buffer, zoraTunicColor.r, adultTunicOffsetInZAR + 0x80);
        WriteFloatToBuffer(buffer, zoraTunicColor.g, adultTunicOffsetInZAR + 0xA8);
        WriteFloatToBuffer(buffer, zoraTunicColor.b, adultTunicOffsetInZAR + 0xD0);

        WriteFloatToBuffer(buffer, childTunicColor.r, childTunicOffsetInZAR + 0x70);
        WriteFloatToBuffer(buffer, childTunicColor.g, childTunicOffsetInZAR + 0x88);
        WriteFloatToBuffer(buffer, childTunicColor.b, childTunicOffsetInZAR + 0xA0);
        
        /* Second Player */
        WriteFloatToBuffer(buffer, kokiriTunicColor.r, adultTunicOffsetInZAR_1 + 0x70);
        WriteFloatToBuffer(buffer, kokiriTunicColor.g, adultTunicOffsetInZAR_1 + 0x98);
        WriteFloatToBuffer(buffer, kokiriTunicColor.b, adultTunicOffsetInZAR_1 + 0xC0);

        WriteFloatToBuffer(buffer, goronTunicColor.r, adultTunicOffsetInZAR_1 + 0x78);
        WriteFloatToBuffer(buffer, goronTunicColor.g, adultTunicOffsetInZAR_1 + 0xA0);
        WriteFloatToBuffer(buffer, goronTunicColor.b, adultTunicOffsetInZAR_1 + 0xC8);

        WriteFloatToBuffer(buffer, zoraTunicColor.r, adultTunicOffsetInZAR_1 + 0x80);
        WriteFloatToBuffer(buffer, zoraTunicColor.g, adultTunicOffsetInZAR_1 + 0xA8);
        WriteFloatToBuffer(buffer, zoraTunicColor.b, adultTunicOffsetInZAR_1 + 0xD0);

        WriteFloatToBuffer(buffer, childTunicColor.r, childTunicOffsetInZAR_1 + 0x70);
        WriteFloatToBuffer(buffer, childTunicColor.g, childTunicOffsetInZAR_1 + 0x88);
        WriteFloatToBuffer(buffer, childTunicColor.b, childTunicOffsetInZAR_1 + 0xA0);

        // Write the assets to final destination
        if (!R_SUCCEEDED(res = FSFILE_Write(assetsOut, &bytesWritten, 0, buffer.data(), buffer.size(), FS_WRITE_FLUSH))) {
            return false;
        }
    }

    FSFILE_Close(assetsOut);

    FSUSER_CloseArchive(sdmcArchive);

    return true;
}
