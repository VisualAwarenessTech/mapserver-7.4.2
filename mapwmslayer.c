/**********************************************************************
 * $Id$
 *
 * Project:  MapServer
 * Purpose:  Implementation of WMS CONNECTIONTYPE - client to WMS servers
 * Author:   Daniel Morissette, DM Solutions Group (morissette@dmsolutions.ca)
 *
 **********************************************************************
 * Copyright (c) 2001-2003, Daniel Morissette, DM Solutions Group Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 * $Log$
 * Revision 1.51  2003/04/23 14:21:18  dan
 * Log an error in msDrawWMSLayerLow() if a GetMap request failed.
 *
 * Revision 1.50  2003/03/26 20:24:38  dan
 * Do not call msDebug() unless debug flag is turned on
 *
 * Revision 1.49  2003/01/30 15:53:48  assefa
 * Windows compilation error.
 *
 * Revision 1.48  2003/01/23 21:42:10  dan
 * Support SLD in GetMap requests when wms_style_..._sld metadata is set
 *
 * Revision 1.47  2003/01/15 19:15:18  dan
 * Do not set TIME= parameter in GetMap URL if wms_time metadata is empty
 *
 * Revision 1.46  2003/01/15 19:05:54  dan
 * Fixed the fix from v1.39 - the test on pszFormatsList had been removed
 * instead of the test on pszStylesList
 *
 * Revision 1.45  2003/01/14 04:13:45  dan
 * Added support for WMS AUTO projections
 *
 * Revision 1.44  2002/12/20 16:34:59  dan
 * Added support for wms_time metadata to pass TIME= parameter in WMS requests
 *
 * Revision 1.43  2002/12/17 21:33:54  dan
 * Enable following redirections with libcurl (requires libcurl 7.10.1+)
 *
 * Revision 1.42  2002/12/17 05:30:17  dan
 * Fixed HTTP timeout value (in secs, not msecs) for WMS/WFS requests
 *
 * Revision 1.41  2002/12/16 20:35:00  dan
 * Flush libwww and use libcurl instead for HTTP requests in WMS/WFS client
 *
 * Revision 1.40  2002/12/13 00:57:31  dan
 * Modified WFS implementation to behave more as a real vector data source
 *
 * Revision 1.39  2002/11/27 20:13:49  julien
 * valid only the style (not stylelist) to build the base url
 *
 * Revision 1.38  2002/11/26 00:06:52  dan
 * Removed offsite hack for bug 214.  Not needed any more since bug is fixed.
 *
 * Revision 1.37  2002/11/25 21:50:29  julien
 * add \0 in the isspace validation for epsg in msBuildWMSLayerURL
 *
 * Revision 1.36  2002/11/25 18:41:26  dan
 * Do not overwrite lp->connection in msBuildWMSLayerURL()
 *
 * Revision 1.35  2002/11/21 00:59:31  dan
 * Added msBuildWMSLayerURLBase() to build WMS connection URL from metadata
 *
 * Revision 1.34  2002/11/15 16:32:36  dan
 * Works more often with offiste=RGB:210,220,230 for bug 214 workaround
 *
 * Revision 1.33  2002/11/15 06:15:29  dan
 * Temporary patch for bug 214 (WMS transparency issue)
 *
 * Revision 1.32  2002/10/28 23:07:38  dan
 * Fixed crash in msDrawWMSLayerLow(): free(wldfile) not needed
 *
 * Revision 1.31  2002/10/09 02:29:03  dan
 * Initial implementation of WFS client support.
 *
 * Revision 1.30  2002/09/20 03:44:07  sdlime
 * Swapped map_path for mappath for consistency.
 *
 * Revision 1.29  2002/09/17 13:08:30  julien
 * Remove all chdir() function and replace them with the new msBuildPath function.
 * This have been done to make MapServer thread safe. (Bug 152)
 *
 * Revision 1.28  2002/07/08 03:46:42  dan
 * Finished changes to download WMS layers in parallel when drawing map
 *
 * Revision 1.27  2002/06/26 03:10:43  dan
 * Modified msGetImages() in preparation for support of multiple requests
 * in parrallel
 *
 * Revision 1.26  2002/06/21 18:33:15  frank
 * added support for IMAGEMODE INT16 and FLOAT
 *
 * Revision 1.25  2002/06/11 13:54:08  frank
 * avoid warning
 *
 * Revision 1.24  2002/05/14 14:07:32  assefa
 * Use of ImageObj to be able to output Vector/Raster beside GD.
 *
 * Revision 1.23  2002/03/13 23:45:22  sdlime
 * Added projection support to the GML output code. Re-shuffled the code 
 * to extract the EPSG values for a layer or map into mapproject.c.
 *
 * Revision 1.22  2002/02/08 21:07:27  sacha
 * Added template support to WMS.
 *
 * Revision 1.21  2002/02/01 00:08:36  sacha
 * Move msTmpFile function from mapwmslayer.c to maputil.c
 *
 * Revision 1.20  2002/01/24 18:31:14  dan
 * Use REQUEST=map instead of REQUEST=Map for WMS 1.0.0 requests.
 *
 * Revision 1.19  2002/01/22 23:00:04  dan
 * Added -DENABLE_STDERR_DEBUG in --enable-debug config option to
 * enable/disable msDebug() output to stderr.  Default is disabled.
 *
 * ...
 *
 * Revision 1.1  2001/08/14 21:26:54  dan
 * Initial revision - only loads and draws WMS CONNECTION URL for now.
 *
 **********************************************************************/

#include "map.h"
#include "maperror.h"

#include <time.h>
#include <ctype.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <process.h>
#include <stdio.h>
#endif

#define WMS_V_1_0_0  100
#define WMS_V_1_0_7  107
#define WMS_V_1_1_0  110



#ifdef USE_WMS_LYR
/**********************************************************************
 *                          msBuildWMSLayerURLBase()
 *
 * Build the base of a GetMap or GetFeatureInfo URL using metadata.  
 * The parameters to set are:
 *   VERSION
 *   LAYERS
 *   FORMAT
 *   TRANSPARENT
 *   STYLES
 *   QUERY_LAYERS (for queriable layers only)
 *
 * Returns a reference to a newly allocated string that should be freed 
 * by the caller.
 **********************************************************************/
static char *msBuildWMSLayerURLBase(mapObj *map, layerObj *lp) 
{
    char *pszURL = NULL, *pszNameEnc=NULL, *pszFormatEnc=NULL;
    char *pszStyleEnc=NULL, *pszSLDEnc=NULL, *pszTimeEnc=NULL;
    const char *pszOnlineResource, *pszVersion, *pszName, *pszFormat;
    const char *pszFormatList, *pszStyle, *pszStyleList, *pszTime;
    const char *pszSLD=NULL, *pszVersionKeyword=NULL;
    int nLen;

    pszOnlineResource = msLookupHashTable(lp->metadata, "wms_onlineresource");
    pszVersion =        msLookupHashTable(lp->metadata, "wms_server_version");
    pszName =           msLookupHashTable(lp->metadata, "wms_name");
    pszFormat =         msLookupHashTable(lp->metadata, "wms_format");
    pszFormatList =     msLookupHashTable(lp->metadata, "wms_formatlist");
    pszStyle =          msLookupHashTable(lp->metadata, "wms_style");
    pszStyleList =      msLookupHashTable(lp->metadata, "wms_stylelist");
    pszTime =           msLookupHashTable(lp->metadata, "wms_time");

    if (pszOnlineResource==NULL || pszVersion==NULL || pszName==NULL)
    {
        msSetError(MS_WMSCONNERR, 
                   "One of wms_onlineresource, wms_server_version, wms_name "
                   "metadata is missing in layer %s.  "
                   "Please either provide a valid CONNECTION URL, or provide "
                   "those values in the layer's metadata.\n", 
                   "msBuildWMSLayerURLBase()", lp->name);
        return NULL;
    }

    if (pszStyle==NULL)
    {
        // When no style is selected, use "" which is a valid default.
        pszStyle = "";
    }
    else
    {
        // Was a wms_style_..._sld URL provided?
        char szBuf[100];
        sprintf(szBuf, "wms_style_%.80s_sld", pszStyle);
        pszSLD = msLookupHashTable(lp->metadata, szBuf);

        if (pszSLD)
        {
            // SLD URL is set.  If this defn. came from a map context then
            // the style name may just be an internal name: "Style{%d}" if
            // that's the case then we should not pass this name via the URL
            if (strncmp(pszStyle, "Style{", 6) == 0)
                pszStyle = "";
        }
    }

    if (pszFormat==NULL && pszFormatList==NULL)
    {
        msSetError(MS_WMSCONNERR, 
                   "At least wms_format or wms_formatlist is required for "
                   "layer %s.  "
                   "Please either provide a valid CONNECTION URL, or provide "
                   "those values in the layer's metadata.\n", 
                   "msBuildWMSLayerURLBase()", lp->name);
        return NULL;
    }

    if (pszFormat != NULL)
    {
        pszFormatEnc = msEncodeUrl(pszFormat);
    }
    else
    {
        /* Look for the first format in list that matches */
        char **papszTok;
        int i, n;
        papszTok = split(pszFormatList, ' ', &n);

        for(i=0; pszFormatEnc==NULL && i<n; i++)
        {
            if (0 
#ifdef USE_GD_GIF
                || strcasecmp(papszTok[i], "GIF")
                || strcasecmp(papszTok[i], "image/gif")
#endif
#ifdef USE_GD_PNG
                || strcasecmp(papszTok[i], "PNG")
                || strcasecmp(papszTok[i], "image/png")
#endif
#ifdef USE_GD_JPEG
                || strcasecmp(papszTok[i], "JPEG")
                || strcasecmp(papszTok[i], "image/jpeg")
#endif
#ifdef USE_GD_WBMP
                || strcasecmp(papszTok[i], "WBMP")
                || strcasecmp(papszTok[i], "image/wbmp")
#endif
                )
            {
                pszFormatEnc = msEncodeUrl(papszTok[i]);
            }
        }
        msFreeCharArray(papszTok, n);

        if (pszFormatEnc == NULL)
        {
            msSetError(MS_WMSCONNERR, 
                       "Could not find a format that matches supported input "
                       "formats in wms_formatlist metdata in layer %s.  "
                       "Please either provide a valid CONNECTION URL, or "
                       "provide the required layer metadata.\n", 
                       "msBuildWMSLayerURLBase()", lp->name);
            return NULL;
        }
    }


    // Encode some values and alloc a buffer large enough and build the URL
    pszNameEnc = msEncodeUrl(pszName);
    pszStyleEnc = msEncodeUrl(pszStyle);

    nLen = 200 + strlen(pszOnlineResource) + strlen(pszVersion) + 
        strlen(pszNameEnc)*2 + strlen(pszFormatEnc) + strlen(pszStyleEnc);
    if (pszTime)
    {
        pszTimeEnc = msEncodeUrl(pszTime);
        nLen += strlen(pszTimeEnc)+6;
    }
    if (pszSLD)
    {
        pszSLDEnc = msEncodeUrl(pszSLD);
        nLen += strlen(pszSLDEnc)+5;
    }

    pszURL = (char*)malloc((nLen+1)*sizeof(char*));

    // Start with the onlineresource value and append trailing '?' or '&' 
    // if missing.
    strcpy(pszURL, pszOnlineResource);
    if (strchr(pszURL, '?') == NULL)
        strcat(pszURL, "?");
    else
    {
        char *c;
        c = pszURL+strlen(pszURL)-1;
        if (*c != '?' && *c != '&')
            strcpy(c+1, "&");
    }

    if (strncmp(pszVersion, "1.0.7", 5) < 0)
        pszVersionKeyword = "WMTVER";
    else
        pszVersionKeyword = "VERSION";

    sprintf(pszURL + strlen(pszURL),
            "SERVICE=WMS&%s=%s&LAYERS=%s&FORMAT=%s&TRANSPARENT=TRUE",
            pszVersionKeyword, pszVersion, pszNameEnc, pszFormatEnc);

    if (pszSLD == NULL)
    {
        // STYLES is mandatory if SLD not set
        sprintf(pszURL + strlen(pszURL), "&STYLES=%s", pszStyleEnc);
    }
    else if (strlen(pszStyle) > 0)
    {
        // Both STYLES and SLD are set
        sprintf(pszURL + strlen(pszURL), "&STYLES=%s&SLD=%s", 
                pszStyleEnc, pszSLDEnc);
    }
    else
    {
        // Only SLD is set
        sprintf(pszURL + strlen(pszURL), "&SLD=%s", pszSLDEnc);
    }

    if (msIsLayerQueryable(lp))
    {
        sprintf(pszURL + strlen(pszURL), "&QUERY_LAYERS=%s", pszNameEnc);
    }
    if (pszTime && strlen(pszTime) > 0)
    {
        sprintf(pszURL + strlen(pszURL), "&TIME=%s", pszTimeEnc);
    }

    msFree(pszNameEnc);
    msFree(pszFormatEnc);
    msFree(pszStyleEnc);
    msFree(pszSLDEnc);
    msFree(pszTimeEnc);

    return pszURL;
}

#endif /* USE_WMS_LYR */


/**********************************************************************
 *                          msBuildWMSLayerURL()
 *
 * Build a GetMap or GetFeatureInfo URL.  
 *
 * Returns a reference to a newly allocated string that should be freed 
 * by the caller.
 **********************************************************************/
#define WMS_GETMAP         1
#define WMS_GETFEATUREINFO 2

char *msBuildWMSLayerURL(mapObj *map, layerObj *lp, int nRequestType,
                         int nClickX, int nClickY, int nFeatureCount,
                         const char *pszInfoFormat, rectObj *bbox_ret) 
{
#ifdef USE_WMS_LYR
    char *pszURL = NULL, *pszEPSG = NULL;
    const char *pszVersion, *pszTmp, *pszRequestParam, *pszExceptionsParam;
    rectObj bbox;
    int nVersion;
    
    if (lp->connectiontype != MS_WMS)
    {
        msSetError(MS_WMSCONNERR, "Call supported only for CONNECTIONTYPE WMS",
                   "msBuildWMSLayerURL()");
        return NULL;
    }


/* ------------------------------------------------------------------
 * Find out request version
 * ------------------------------------------------------------------ */
    if (lp->connection == NULL ||
        ((pszVersion = strstr(lp->connection, "VERSION=")) == NULL &&
         (pszVersion = strstr(lp->connection, "version=")) == NULL &&
         (pszVersion = strstr(lp->connection, "WMTVER=")) == NULL &&
         (pszVersion = strstr(lp->connection, "wmtver=")) == NULL ) )
    {
        // CONNECTION missing or seems incomplete... try to build from metadata
        pszURL = msBuildWMSLayerURLBase(map, lp);
        if (pszURL == NULL)
            return NULL;  // An error already produced.
    }
    else
    {
        // CONNECTION string seems complete, start with that.
        pszURL = strdup(lp->connection);
    }

    if ((pszVersion = strstr(pszURL, "VERSION=")) == NULL &&
        (pszVersion = strstr(pszURL, "version=")) == NULL &&
        (pszVersion = strstr(pszURL, "WMTVER=")) == NULL &&
        (pszVersion = strstr(pszURL, "wmtver=")) == NULL ) 
    {
        msSetError(MS_WMSCONNERR, "WMS Connection String must contain the VERSION or WMTVER parameter (with name in uppercase).", "msBuildWMSLayerURL()");
        return NULL;
    }
    pszVersion = strchr(pszVersion, '=')+1;
    if (strncmp(pszVersion, "1.0.8", 5) >= 0)    /* 1.0.8 == 1.1.0 */
        nVersion = WMS_V_1_1_0;
    else if (strncmp(pszVersion, "1.0.7", 5) >= 0)
        nVersion = WMS_V_1_0_7;
    else if (strncmp(pszVersion, "1.0.0", 5) >= 0)
        nVersion = WMS_V_1_0_0;
    else
    {
        msSetError(MS_WMSCONNERR, "MapServer supports only WMS 1.0.0 to 1.1.0 (please verify the VERSION parameter in the connection string).", "msBuildWMSLayerURL()");
        return NULL;
    }


/* ------------------------------------------------------------------
 * For GetFeatureInfo requests, make sure QUERY_LAYERS is included
 * ------------------------------------------------------------------ */
    if  (nRequestType == WMS_GETFEATUREINFO &&
         strstr(pszURL, "QUERY_LAYERS=") == NULL &&
         strstr(pszURL, "query_layers=") == NULL )
    {
        msSetError(MS_WMSCONNERR, "WMS Connection String must contain the QUERY_LAYERS parameter to support GetFeatureInfo requests (with name in uppercase).", "msBuildWMSLayerURL()");
        return NULL;
    }



/* ------------------------------------------------------------------
 * Figure the SRS we'll use for the request.
 * - Fetch the map SRS (if it's EPSG)
 * - Check if map SRS is listed in layer wms_srs metadata
 * - If map SRS is valid for this layer then use it
 * - Otherwise request layer in its default SRS and we'll reproject later
 * ------------------------------------------------------------------ */
    if ((pszEPSG = (char*)msGetEPSGProj(&(map->projection), 
                                           NULL, MS_TRUE)) != NULL &&
        (pszEPSG = strdup(pszEPSG)) != NULL &&
        (strncasecmp(pszEPSG, "EPSG:", 5) == 0 ||
         strncasecmp(pszEPSG, "AUTO:", 5) == 0) )
    {
        const char *pszLyrEPSG, *pszFound;
        int nLen;
        char *pszPtr = NULL;

        // If it's an AUTO projection then keep only id and strip off 
        // the parameters for now (we'll restore them at the end)
        if (strncasecmp(pszEPSG, "AUTO:", 5) == 0)
        {
            if ((pszPtr = strchr(pszEPSG, ',')))
                *pszPtr = '\0';
        }

        nLen = strlen(pszEPSG);

        pszLyrEPSG = msGetEPSGProj(&(lp->projection), lp->metadata, MS_FALSE);

        if (pszLyrEPSG == NULL ||
            (pszFound = strstr(pszLyrEPSG, pszEPSG)) == NULL ||
            ! ((*(pszFound+nLen) == '\0') || isspace(*(pszFound+nLen))) )
        {
            // Not found in Layer's list of SRS (including projection object)
            free(pszEPSG);
            pszEPSG = NULL;
        }
        if (pszEPSG && pszPtr)
            *pszPtr = ',';  // Restore full AUTO:... definition
    }

    if (pszEPSG == NULL &&
        ((pszEPSG = (char*)msGetEPSGProj(&(lp->projection), 
                                            lp->metadata, MS_TRUE)) == NULL ||
         (pszEPSG = strdup(pszEPSG)) == NULL ||
         (strncasecmp(pszEPSG, "EPSG:", 5) != 0 &&
          strncasecmp(pszEPSG, "AUTO:", 5) != 0 ) ) )
    {
        msSetError(MS_WMSCONNERR, "Layer must have an EPSG or AUTO projection code (in its PROJECTION object or wms_srs metadata)", "msBuildWMSLayerURL()");
        if (pszEPSG) free(pszEPSG);
        return NULL;
    }

/* ------------------------------------------------------------------
 * For an AUTO projection, set the Units,lon0,lat0 if not already set
 * ------------------------------------------------------------------ */
    if (strncasecmp(pszEPSG, "AUTO:", 5) == 0 &&
        strchr(pszEPSG, ',') == NULL)
    {
        pointObj oPoint;
        char *pszNewEPSG;

        // Use center of map view for lon0,lat0
        oPoint.x = (map->extent.minx + map->extent.maxx)/2.0;
        oPoint.y = (map->extent.miny + map->extent.maxy)/2.0;
        msProjectPoint(&(map->projection), &(map->latlon), &oPoint);

        pszNewEPSG = (char*)malloc(101*sizeof(char));
#if defined(_WIN32) && !defined(__CYGWIN__)
        sprintf(pszNewEPSG, "%s,9001,%.16g,%.16g", 
                pszEPSG, oPoint.x, oPoint.y);
#else
        snprintf(pszNewEPSG, 100, "%s,9001,%.16g,%.16g", 
                 pszEPSG, oPoint.x, oPoint.y);
#endif        
        pszNewEPSG[100]='\0';
        free(pszEPSG);
        pszEPSG=pszNewEPSG;
    }

/* ------------------------------------------------------------------
 * Set layer SRS and reproject map extents to the layer's SRS
 * ------------------------------------------------------------------ */
    // No need to set lp->proj if it's already set to the right EPSG code
    if ((pszTmp = msGetEPSGProj(&(lp->projection), NULL, MS_TRUE)) == NULL ||
        strcasecmp(pszEPSG, pszTmp) != 0)
    {
        if (strncasecmp(pszEPSG, "EPSG:", 5) == 0)
        {
            char szProj[20];
            sprintf(szProj, "init=epsg:%s", pszEPSG+5);
            if (msLoadProjectionString(&(lp->projection), szProj) != 0)
                return NULL;
        }
        else
        {
            if (msLoadProjectionString(&(lp->projection), pszEPSG) != 0)
                return NULL;
        }
    }

    bbox = map->extent;
    if (msProjectionsDiffer(&(map->projection), &(lp->projection)))
    {
        msProjectRect(&(map->projection), &(lp->projection), &bbox);
    }

    if (bbox_ret != NULL)
        *bbox_ret = bbox;

/* ------------------------------------------------------------------
 * Build the request URL.
 * At this point we set only the following parameters for GetMap:
 *   REQUEST
 *   SRS
 *   BBOX
 *
 * And for GetFeatureInfo:
 *   X
 *   Y
 *   INFO_FORMAT
 *   FEATURE_COUNT (only if nFeatureCount > 0)
 *
 * The connection string should contain all other required params, 
 * including:
 *   VERSION
 *   LAYERS
 *   FORMAT
 *   TRANSPARENT
 *   STYLES
 *   QUERY_LAYERS (for queriable layers only)
 * ------------------------------------------------------------------ */
    // Make sure we have a big enough buffer for the URL
    if(!(pszURL = (char *)realloc(pszURL, (strlen(pszURL)+512)*sizeof(char)))) 
    {
        msSetError(MS_MEMERR, NULL, "msBuildWMSLayerURL()");
        return NULL;
    }

    // __TODO__ We have to urlencode each value... especially the BBOX values
    // because if they end up in exponent format (123e+06) the + will be seen
    // as a space by the remote server.

    if (nRequestType == WMS_GETFEATUREINFO)
    {
        char szFeatureCount[30] = "";

        if (nVersion >= WMS_V_1_0_7)
            pszRequestParam = "GetFeatureInfo";
        else
            pszRequestParam = "feature_info";

        if (nVersion >= WMS_V_1_1_0)
            pszExceptionsParam = "application/vnd.ogc.se_xml";
        else if (nVersion > WMS_V_1_1_0)  /* 1.0.1 to 1.0.7 */
            pszExceptionsParam = "SE_XML";
        else
            pszExceptionsParam = "WMS_XML";

        // If FEATURE_COUNT <= 0 then don't pass this parameter
        // The spec states that FEATURE_COUNT must be greater than zero
        // and if not passed then the behavior is up to the server
        if (nFeatureCount > 0)
        {
            sprintf(szFeatureCount, "&FEATURE_COUNT=%d", nFeatureCount);
        }

        sprintf(pszURL + strlen(pszURL), 
                "&REQUEST=%s&WIDTH=%d&HEIGHT=%d&SRS=%s&BBOX=%f,%f,%f,%f"
                "&EXCEPTIONS=%s&X=%d&Y=%d&INFO_FORMAT=%s%s",
                pszRequestParam, map->width, map->height, 
                pszEPSG, bbox.minx, bbox.miny, bbox.maxx, bbox.maxy,
                pszExceptionsParam,
                nClickX, nClickY, pszInfoFormat, szFeatureCount);
    }
    else /* if (nRequestType == WMS_GETMAP) */
    {
        if (nVersion >= WMS_V_1_0_7)
            pszRequestParam = "GetMap";
        else
            pszRequestParam = "map";

        if (nVersion >= WMS_V_1_1_0)
            pszExceptionsParam = "application/vnd.ogc.se_inimage";
        else
            pszExceptionsParam = "INIMAGE";

        sprintf(pszURL + strlen(pszURL), 
                "&REQUEST=%s&WIDTH=%d&HEIGHT=%d&SRS=%s&BBOX=%f,%f,%f,%f"
                "&EXCEPTIONS=%s",
                pszRequestParam, map->width, map->height, 
                pszEPSG, bbox.minx, bbox.miny, bbox.maxx, bbox.maxy,
                pszExceptionsParam);
    }

    free(pszEPSG);

    return pszURL;

#else
/* ------------------------------------------------------------------
 * WMS CONNECTION Support not included...
 * ------------------------------------------------------------------ */
  msSetError(MS_WMSCONNERR, "WMS CLIENT CONNECTION support is not available.", 
             "msBuildWMSLayerURL()");
  return NULL;

#endif /* USE_WMS_LYR */

}


/**********************************************************************
 *                          msWMSGetFeatureInfoURL()
 *
 * Build a GetFeatureInfo URL for this layer.
 *
 * Returns a reference to a newly allocated string that should be freed 
 * by the caller.
 **********************************************************************/
char *msWMSGetFeatureInfoURL(mapObj *map, layerObj *lp,
                             int nClickX, int nClickY, int nFeatureCount,
                             const char *pszInfoFormat) 
{
    return msBuildWMSLayerURL(map, lp, WMS_GETFEATUREINFO,
                              nClickX, nClickY, nFeatureCount,
                              pszInfoFormat, NULL);
}


/**********************************************************************
 *                          msPrepareWMSLayerRequest()
 *
 **********************************************************************/

int msPrepareWMSLayerRequest(int nLayerId, mapObj *map, layerObj *lp,
                             httpRequestObj *pasReqInfo, int *numRequests) 
{
#ifdef USE_WMS_LYR
    char *pszURL = NULL;
    const char *pszTmp;
    rectObj bbox;
    int nTimeout;

    if (lp->connectiontype != MS_WMS)
        return MS_FAILURE;

/* ------------------------------------------------------------------
 * Build the request URL, this will also set layer projection and
 * compute BBOX in that projection.
 * ------------------------------------------------------------------ */
    if ((pszURL = msBuildWMSLayerURL(map, lp, WMS_GETMAP,
                                     0, 0, 0, NULL, &bbox)) == NULL)
    {
        /* an error was already reported. */
        return MS_FAILURE;
    }

/* ------------------------------------------------------------------
 * Check if layer overlaps current view window (using wms_latlonboundingbox)
 * ------------------------------------------------------------------ */
    if ((pszTmp = msLookupHashTable(lp->metadata, 
                                    "wms_latlonboundingbox")) != NULL)
    {
        char **tokens;
        int n;
        rectObj ext;

        tokens = split(pszTmp, ' ', &n);
        if (tokens==NULL || n != 4) {
            msSetError(MS_WMSCONNERR, "Wrong number of arguments for 'wms_latlonboundingbox' metadata.",
                       "msDrawWMSLayer()");
            free(pszURL);
            return MS_FAILURE;
        }

        ext.minx = atof(tokens[0]);
        ext.miny = atof(tokens[1]);
        ext.maxx = atof(tokens[2]);
        ext.maxy = atof(tokens[3]);

        msFreeCharArray(tokens, n);

        // Reproject latlonboundingbox to the selected SRS for the layer and
        // check if it overlaps the bbox that we calculated for the request

        msProjectRect(&(map->latlon), &(lp->projection), &ext);
        if (!msRectOverlap(&bbox, &ext))
        {
            // No overlap... nothing to do
            free(pszURL);

            return MS_SUCCESS;  // No overlap.
        }
    }

/* ------------------------------------------------------------------
 * check to see if a the metadata wms_connectiontimeout is set. If it is 
 * the case we will use it, else we use the default which is 30 seconds.
 * First check the metadata in the layer object and then in the map object.
 * ------------------------------------------------------------------ */
    nTimeout = 30;  // Default is 30 seconds 
    if ((pszTmp = msLookupHashTable(lp->metadata, 
                                    "wms_connectiontimeout")) != NULL)
    {
        nTimeout = atoi(pszTmp); 
    }
    else if ((pszTmp = msLookupHashTable(map->web.metadata, 
                                         "wms_connectiontimeout")) != NULL)
    {
        nTimeout = atoi(pszTmp);
    }

/* ------------------------------------------------------------------
 * Add a request to the array (already preallocated)
 * ------------------------------------------------------------------ */
    pasReqInfo[(*numRequests)].nLayerId = nLayerId;
    pasReqInfo[(*numRequests)].pszGetUrl = pszURL;
    pszURL = NULL;
    // We'll store the remote server's response to a tmp file.
    pasReqInfo[(*numRequests)].pszOutputFile = msTmpFile(map->web.imagepath, 
                                                         "img.tmp");
    pasReqInfo[(*numRequests)].nStatus = 0;
    pasReqInfo[(*numRequests)].nTimeout = nTimeout;
    pasReqInfo[(*numRequests)].bbox = bbox;
    pasReqInfo[(*numRequests)].debug = lp->debug;

    (*numRequests)++;

    return MS_SUCCESS;

#else
/* ------------------------------------------------------------------
 * WMS CONNECTION Support not included...
 * ------------------------------------------------------------------ */
  msSetError(MS_WMSCONNERR, "WMS CLIENT CONNECTION support is not available.", 
             "msDrawWMSLayer()");
  return(MS_FAILURE);

#endif /* USE_WMS_LYR */

}

/**********************************************************************
 *                          msDrawWMSLayerLow()
 *
 **********************************************************************/

int msDrawWMSLayerLow(int nLayerId, httpRequestObj *pasReqInfo, 
                      int numRequests, mapObj *map, layerObj *lp, imageObj *img) 
{
#ifdef USE_WMS_LYR
    int status = MS_SUCCESS;
    int iReq = -1;
    char szPath[MS_MAXPATHLEN];

/* ------------------------------------------------------------------
 * Find the request info for this layer in the array, based on nLayerId
 * ------------------------------------------------------------------ */
    for(iReq=0; iReq<numRequests; iReq++)
    {
        if (pasReqInfo[iReq].nLayerId == nLayerId)
            break;
    }

    if (iReq == numRequests)
        return MS_SUCCESS;  // This layer was skipped... nothing to do.

    if ( !MS_HTTP_SUCCESS( pasReqInfo[iReq].nStatus ) )
    {
/* ==================================================================== 
      Failed downloading layer... we log an error but we still return 
      SUCCESS here so that the layer is only skipped intead of aborting
      the whole draw map.
 ==================================================================== */
        msSetError(MS_WMSERR, 
                   "WMS GetMap request failed with status %d for layer '%s'.",
                   "msDrawWMSLayerLow()", 
                   pasReqInfo[iReq].nStatus, (lp->name?lp->name:"(null)") );

        return MS_SUCCESS;
    }

/* ------------------------------------------------------------------
 * Prepare layer for drawing, reprojecting the image received from the
 * server if needed...
 * ------------------------------------------------------------------ */
    lp->type = MS_LAYER_RASTER;

    if (lp->data) free(lp->data);
    lp->data =  strdup(pasReqInfo[iReq].pszOutputFile);

    if (!msProjectionsDiffer(&(map->projection), &(lp->projection)))
    {
        // The simple case... no reprojection needed... render layer directly.
        lp->transform = MS_FALSE;
        if (msDrawRasterLayerLow(map, lp, img) != 0)
            status = MS_FAILURE;
    }
    else
    {
        FILE *fp;
        char *wldfile;
        // OK, we have to resample the raster to map projection...
        lp->transform = MS_TRUE;

        // Create a world file with raster extents
        // One line per value, in this order: cx, 0, 0, cy, ulx, uly
        wldfile = msBuildPath(szPath, lp->map->mappath, lp->data);
        if (wldfile)    
            strcpy(wldfile+strlen(wldfile)-3, "wld");
        if (wldfile && (fp = fopen(wldfile, "wt")) != NULL)
        {
            fprintf(fp, "%f\n", MS_CELLSIZE(pasReqInfo[iReq].bbox.minx,
                                            pasReqInfo[iReq].bbox.maxx, 
                                            map->width));
            fprintf(fp, "0\n");
            fprintf(fp, "0\n");
            fprintf(fp, "%f\n", MS_CELLSIZE(pasReqInfo[iReq].bbox.maxy,
                                            pasReqInfo[iReq].bbox.miny, 
                                            map->height));
            fprintf(fp, "%f\n", pasReqInfo[iReq].bbox.minx);
            fprintf(fp, "%f\n", pasReqInfo[iReq].bbox.maxy);
            fclose(fp);

            // GDAL should be called to reproject automatically.
            if (msDrawRasterLayerLow(map, lp, img) != 0)
                status = MS_FAILURE;

            unlink(wldfile);
        }
        else
        {
            msSetError(MS_WMSCONNERR, 
                       "Unable to create wld file for WMS slide.", 
                       "msDrawWMSLayer()");
            status = MS_FAILURE;
        }

    } 

    // We're done with the remote server's response... delete it.
    unlink(lp->data);

    free(lp->data);
    lp->data = NULL;

    return status;

#else
/* ------------------------------------------------------------------
 * WMS CONNECTION Support not included...
 * ------------------------------------------------------------------ */
  msSetError(MS_WMSCONNERR, "WMS CLIENT CONNECTION support is not available.", 
             "msDrawWMSLayer()");
  return(MS_FAILURE);

#endif /* USE_WMS_LYR */

}


