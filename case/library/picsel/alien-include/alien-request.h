/**
 * Requests for User Input.
 *
 * These functions must be implemented by the Alien Application integrator,
 * before the application can be linked with the Picsel library. If these
 * features are not required by the Alien Application, stub functions should be
 * provided which compile but simply return errors. Please see the integration
 * guide section on optional function implementations for more information
 * about this process.
 *
 * @file
 * $Id: alien-request.h,v 1.108 2013/06/05 13:41:16 neilk Exp $
 */
/* Copyright (C) Picsel, 2005-2008. All Rights Reserved. */

#ifndef ALIEN_REQUEST_H
#define ALIEN_REQUEST_H

#include "alien-types.h"
#include "alien-notify.h" /* For AlienInformation_UrlAuthenticationInfo */
#include "alien-request-browser.h" /* Separated for clarity */
#include "picsel-print.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup TgvContentRequests Requests for User Input
 * @ingroup TgvNotify
 *
 * A request from the Picsel Library to the Alien Appliction is sent when the
 * Picsel Library needs confirmation that it is ok to proceed with a
 * particular operation; for example loading a new page, or authenticating
 * with a server.
 *
 * The Picsel Library will call AlienUserRequest_request() with a pointer to
 * a @ref PicselUserRequest_Request struct.
 *
 * This struct contains a @ref PicselUserRequest_Type, and a union of
 * many possible structs. The member of the union may, or may not, have
 * fields within it which have been pre-filled by the Picsel Library.
 *
 * Once @ref AlienUserRequest_request has returned, the Picsel Library will
 * free up the memory associated with any fields within the active @ref
 * PicselUserRequest_Request.requestData union member. For this reason, it
 * is important that the Alien Application creates copies of any fields which
 * it will later need access to.
 *
 * The Alien Application may then process the request for user input, either
 * by prompting a user for information, or using information defined by a
 * policy created by either the end user or by the integrator themselves. When
 * filling in fields within the struct, it is important that the Alien
 * Application retains pointers to those fields so the memory they point to
 * can later be freed.  The @c requestData struct itself will have been freed
 * by the Picsel library, so its members cannot be accessed.
 *
 * Once the fields within the struct have been completed, the Alien
 * Application should call PicselUserRequest_notify(), with a pointer to
 * the same struct, now with the necessary fields completed.
 *
 * When PicselUserRequest_notify() returns, the Alien Application can now free
 * the memory associated with the fields in the struct.
 */

/**
 * @ingroup TgvContentRequests
 * Requests, Questions and Confirmations from the Picsel Library to the Alien
 * Application.
 *
 * Sometimes the Picsel Library will need further information before it can
 * continue, such as a string to search for within a document, or confirmation
 * that it is okay to move from one page to another, etc.
 *
 * When this happens, the Picsel Library will call AlienUserRequest_request()
 * with a data structure containing more details.
 *
 * In some cases, the Alien Application may prompt the user for input; in
 * others, it may be able to make the decision without presenting a user
 * dialogue box - perhaps by consulting a per-user or per-device policy.
 *
 * @par Memory and data conservation
 *
 * The Picsel Library will copy any relevant data that you provide in
 * PicselUserRequest_notify(), and so your Alien application may free that
 * memory when PicselUserRequest_notify() returns.
 *
 * Similarly, the Alien Application must copy any string or buffer data provided
 * to you by the Picsel Library via AlienUserRequest_request(), as the memory
 * will be deallocated by Picsel when AlienUserRequest_request() returns.
 */
typedef enum PicselUserRequest_Type
{
    PicselUserRequest_Type_Transition = (1<<16), /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_Authentication,       /* Documented in alien-request-browser.h */

    /**
     * A request for a password to access a protected document. This request may
     * have a reject option.
     *
     * See @ref PicselUserRequest_Password for a description of the struct
     * sent along with this type of request.
     */
    PicselUserRequest_Type_Document_Password,


    PicselUserRequest_Type_Notify,             /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_Query,              /* Documented in alien-request-browser.h */

    /**
     * Request a string from the user.
     *
     * See PicselUserRequest_String for a description of the struct sent
     * along with this type of request.
     */
    PicselUserRequest_Type_String,

    /**
     * Request the contents of the platforms clipboard as a string.
     *
     * See PicselUserRequest_GetClipboard for a description of the struct sent
     * along with this type of request.
     */
    PicselUserRequest_Type_GetClipboard,

    /**
     * Request an image filepath from the platform.
     *
     * See PicselUserRequest_GetImage for a description of the struct sent
     * along with this type of request.
     */
    PicselUserRequest_Type_GetImage,

    /**
     * Request a filepath to save the current document to from the platform.
     *
     * See PicselUserRequest_GetSavePath for a description of the struct sent
     * along with this type of request.
     */
    PicselUserRequest_Type_GetSavePath,

    PicselUserRequest_Type_Cookie,             /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_HttpReferrer,       /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_Certificate,        /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_Redirect,           /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_InsecureObjects,    /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_ChooseFileName,     /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_ValidateFileName,   /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_AntiVirus_URL,      /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_AntiVirus_Content,  /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_FormAutoComplete,   /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_FileSaveContentLoss,/* Documented in alien-request-browser.h */
    PicselUserRequest_Type_DownloadConfirm,    /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_DownloadData,       /* Documented in alien-request-browser.h */

    /**
     * The Alien Application is trying to perform an operation on the
     * Contacts list.
     *
     * A @ref PicselUserRequest_ContactsOperation struct will be sent along
     * with this value.
     *
     * See @ref PicselUserRequest_ContactsOperation for a description of the struct
     * sent along with this type of request.
     */
    PicselUserRequest_Type_ContactsOperation,

    /**
     * Request a date/time from the user. The Alien Application could, for
     * example, display a calendar/clock type control to the user.
     *
     * Along with this value, the Picsel Library will send a @ref
     * PicselUserRequest_DateTime struct which contains one of @ref
     * PicselUserRequest_DateTime_Granularity. This will tell the Alien
     * Application if a date, or a date + time, is required.
     */
    PicselUserRequest_Type_DateTime,

    PicselUserRequest_Type_OpenFolder,       /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_SecretModeChange, /* Documented in alien-request-browser.h */
    PicselUserRequest_Type_CreateBookmark,   /* Documented in alien-request-browser.h */

    /**
     * This request will be sent if single page loading is enabled, and a
     * search is performed on a document that supports single page loading.
     * It will be sent to confirm whether search should proceed onto the next
     *  page.  This will occur if the search term was not found on the
     * current page.  Accepting the request by setting
     * @ref PicselUserRequest_Request.result to @ref
     * PicselUserRequest_Result_Accepted will allow the search to continue.
     * Rejecting the request will stop the search, and remain on the current
     * page.
     *
     * See @ref PicselUserRequest_SearchNextPage for a description of the
     * struct sent along with this type of request.
     */
    PicselUserRequest_Type_SearchNextPage,

    /**
     * PicselUserRequest_Type_DrmInfo. This request will be sent when DRM
     *    (Digital Rights Management) protected content is being accessed.
     *    It will be sent with a PicselUserRequest_DrmInfo_OperationType value
     *    of PicselUserRequest_DrmInfo_Validation when an attempt is made to
     *    open DRM protected content.  The Alien application must then decide
     *    whether the attempt is valid or invalid.
     *    Further requests will be sent when the display of the content starts,
     *    is paused, or stops (i.e. the content is closed and removed from the
     *    display).  These requests will have a
     *    PicselUserRequest_DrmInfo_OperationType of
     *    PicselUserRequest_DrmInfo_Consumption.  The Alien application should
     *    then decide whether and how these accesses consume limited DRM rights,
     *    such as a total allowed number of content plays, or a time-limited
     *    license to display the protected content.
     *
     * See @ref PicselUserRequest_DrmInfo for a description of the
     * struct sent along with this type of request.
     */
    PicselUserRequest_Type_DrmInfo,

    /**
     * Request the user edit an excel cell.
     *
     * See @ref PicselUserRequest_ExcelCellEdit for a description of the struct sent
     * along with this type of request.
     */
    PicselUserRequest_Type_ExcelCellEdit,

    /**
     * Request the host initiate a printing session of the document and the
     * specified pages
     *
     * See @ref PicselUserRequest_PrintDocument for a description of the struct sent
     * along with this type of request.
     */
    PicselUserRequest_Type_PrintDocument,

    /**
     * Request the host initiate a message editing session
     *
     * See @ref PicselUserRequest_MessageEdit for a description of the struct
     * sent along with this type of request.
     */
    PicselUserRequest_Type_MessageEdit,

    /**
     * Request the host search for available printers
     *
     * See @ref PicselUserRequest_SearchForPrinters for a description of the
     * struct sent along with this type of request.
     */
    PicselUserRequest_Type_SearchForPrinters,

    /**
     * Request the host to handle uploading a file to the cloud
     *
     * See @ref PicselUserRequest_UploadDocument for a description of the
     * struct sent along with this type of request.
     */
    PicselUserRequest_Type_UploadDocument,

    /**
     * Request the host to share the specified document with external
     * applications.
     *
     * See @ref PicselUserRequest_ShareDocument for a description of the
     * struct sent along with this type of request.
     */
    PicselUserRequest_Type_ShareDocument,

    /**
     * Used internally by the Picsel Library. Do not use this request type.
     */
    PicselUserRequest__Last
}
PicselUserRequest_Type;

/* **************************** Get Clipboard ****************************** */

/**
 * @ingroup TgvGetClipboard
 *
 * Type of clipboard data to get
 *
 * This value forms part of the struct, @ref PicselUserRequest_GetClipboard, sent
 * along with a @ref PicselUserRequest_Type_GetClipboard event.
 */
typedef enum PicselUserRequest_GetClipboardType
{
    PicselUserRequest_GetClipboardType_Text  /* A generic string request */
}
PicselUserRequest_GetClipboardType;

/**
 * @ingroup TgvGetClipboard
 *
 * Struct to hold the string returned by the PicselUserRequest_Type_GetClipboard
 *
 * This struct should be sent along with a @ref PicselUserRequest_Type_GetClipboard
 * request.
 */
typedef struct PicselUserRequest_GetClipboard
{
    /**
     *  [out]   Pointer to the clipboard text to be set by the alien.
     */
    char       *string;
}
PicselUserRequest_GetClipboard;

/* **************************** Get Image ****************************** */
/**
 * @ingroup TgvGetImage
 *
 * Type of Image to get, may be from the filesystem or the device's camera.
 *
 * This value forms part of the struct, @ref PicselUserRequest_GetImage, sent
 * along with a @ref PicselUserRequest_Type_GetImage event.
 */
typedef enum PicselUserRequest_ImageType
{
    /* Return a filepath to an image that the user has selected from the
       device's filesystem */
    PicselUserRequest_ImageType_Filesystem,

    /* The alien will start the camera app and allow the user to take a picture.
       The picture will be saved to the filesystem and its filepath will
       be returned. */
    PicselUserRequest_ImageType_Camera
}
PicselUserRequest_ImageType;

/**
 * @ingroup TgvGetImage
 *
 * Struct to hold the filepath string returned by the
 * PicselUserRequest_Type_GetImage
 *
 * This struct should be sent along with a @ref PicselUserRequest_Type_GetImage
 * request.
 */
typedef struct PicselUserRequest_GetImage
{
     /**
     *  [in]    Type of image to get. May be from the filesystem
     *          or from the camera.
     */
    PicselUserRequest_ImageType    type;

    /**
     *  [out]   Filepath of the image returned by the Alien.
     */
    char    *image;
}
PicselUserRequest_GetImage;

/* **************************** Get Save Path ****************************** */

/* @ingroup TgvGetSavePath
 *
 * Struct to hold the suggested filename and the filepath string returned by the
 * PicselUserRequest_Type_GetSavePath
 *
 * This struct should be sent along with a @ref PicselUserRequest_Type_GetSavePath
 * request.
 */
typedef struct PicselUserRequest_GetSavePath
{
     /**
     *  [in]    The current or suggested filename to use when constructing a
     *          filepath to use for saving the current document.
     */
    const char    *suggestedFileName;

    /**
     *  [out]   Filepath to use when saving the current document.
     */
    char    *returnedFilePath;
}
PicselUserRequest_GetSavePath;

/* **************************** Password Input ****************************** */

/**
 * @addtogroup TgvDocPassword Document Password Entry
 * @ingroup TgvContentRequests
 *
 * The Picsel Library can open documents such as PDF, which are protected
 * by user passwords.
 */

/**
 * @ingroup TgvDocPassword
 *
 * Struct sent along with a @ref PicselUserRequest_Type_Document_Password
 * request.
 */
typedef struct PicselUserRequest_Password
{
    const char *passwordDefault; /**< [in]  default for password    */
          char *password;        /**< [out] user's entered password */
}
PicselUserRequest_Password;

/* *************************** Contact Operations *************************** */

/**
 * @ingroup TgvContactInfo
 *
 * Sent as part of a @ref PicselUserRequest_ContactsOperation struct, this
 * value tells the Picsel Library which sort of operation is to be carried out
 * on the Contacts Database.
 */
typedef enum PicselUserRequest_ContactsOperation_Type
{
    /**
     * Select a contact from the database.
     */
    PicselUserRequest_ContactsOperation_Select = (1<<16),
    /**
     * Create a new contact from the data in @ref PicselContacts_Field.
     */
    PicselUserRequest_ContactsOperation_Create,
    /**
     * Update an exising contact, using the data in @ref PicselContacts_Field.
     */
    PicselUserRequest_ContactsOperation_Update
}
PicselUserRequest_ContactsOperation_Type;


/**
 * @ingroup TgvContactInfo
 *
 * Information about a request to edit/add a contact. This is used for
 * @ref PicselUserRequest_Type_ContactsOperation. A contacts operation
 * structure contains the type of the operation (create, edit, or select) as
 * well as operation specific parameters and result data.
 */
typedef struct PicselUserRequest_ContactsOperation
{
    /**
     * The operation to carry out on the database (select/insert/update).
     */
    PicselUserRequest_ContactsOperation_Type  type;

    /**
     * The contact field to search for.  Required for update operation. NULL for
     * other operations.
     */
    struct PicselContacts_Field              *searchField;

    /**
     * The contact field to change.  Required for create. Optional for update.
     * NULL for select.
     */
    struct PicselContacts_Field              *changeField;

    /**
     * The contact info. Only filled for select operations.
     */
    struct PicselContacts_ContactInfo        *result;
}
PicselUserRequest_ContactsOperation;



/* ******************************** Date & Time ******************************** */

/**
 * @ingroup TgvTimeOfDay
 *
 * The Picsel Library requires a date to be supplied by either the Alien
 * Application or the device user.
 *
 * Specifies the accuracy of date/time data required. Used in the response field
 * of PicselUserRequest_DateTime to allow the Picsel Library to request a
 * calendar or similar dialogue to be displayed, for the user to select a date
 * and/or time.
 */
typedef enum PicselUserRequest_DateTime_Granularity
{
    /**
     * A date is required but the exact time within that day is unimportant,
     * such as an anniversary.
     */
    PicselUserRequest_DateTime_Date     = (1<<16),
    /**
     * A date is required and the exact time within that day is important,
     * such as a meeting.
     */
    PicselUserRequest_DateTime_DateTime
}
PicselUserRequest_DateTime_Granularity;

/**
 * @ingroup TgvTimeOfDay
 *
 * Used to request the device user to supply a time, accurate to @ref
 * PicselUserRequest_DateTime_Granularity.
 */
typedef struct PicselUserRequest_DateTime
{
    /**
     * How accurate the time requested should be.
     */
    PicselUserRequest_DateTime_Granularity  granularity;

    /**
     * [out] Date & time (seconds since 1st Jan 1970) Coordinated Universal Time
     * (UTC).
     */
    unsigned long                           dateTime;
}
PicselUserRequest_DateTime;

/* *********************** SearchNextPage *********************************** */
/**
 * @ingroup TgvSearch
 *
 * Allow the Picsel Library to search another page of a partially loaded
 * document.
 *
 * The Alien Application has finished searching the current page. There is
 * another page available, but it hasn't been loaded into memory yet. Should the
 * Picsel Library load the next page and continue searching?
 *
 * This could be decided by the Alien Application, or a prompt could be
 * displayed to the device user, allowing them to decide.
 *
 * To continue searching, the Alien Application should accept the request by
 * sending @ref PicselUserRequest_Result_Accepted. To cancel searching, send
 * @ref PicselUserRequest_Result_Rejected.
 *
 * @ref PicselUserRequest_SearchNextPage will request can only be sent if @ref
 * PicselConfigFV_singlePageLoad has been set.
 *
 * Passed along with a @ref PicselUserRequest_Type_SearchNextPage event type.
 */
typedef struct PicselUserRequest_SearchNextPage
{
    int     page;   /**< [in]  Current page number in the document. */
}
PicselUserRequest_SearchNextPage;

/*************************** DrmInfo ************************************/

/**
 * @ingroup TgvDrmFiles
 *
 * Operation type for DRM Information.
 *
 * Used in the response field of PicselUserRequest_Type_DrmInfo
 * to allow Picsel to request DRM validation or DRM consumption
 * and to get the DRM information of the content.
 */
typedef enum PicselUserRequest_DrmInfo_OperationType
{
    /** Used to identify an initial request to validate access to a DRM
     *  protected resource. */
    PicselUserRequest_DrmInfo_Validation  = (1<<16),

    /** Used to identify a request to consume limited DRM access rights to
     *  content. */
    PicselUserRequest_DrmInfo_Consumption
}
PicselUserRequest_DrmInfo_OperationType;

/**
 * @ingroup TgvDrmFiles
 * A type specifying the DRM consumption event being requested.
 */
typedef enum PicselUserRequest_DrmInfo_ConsumptionEvent
{
    /** The DRM content has started to be displayed. */
    PicselUserRequest_DrmInfo_ConsumeStart = (1<<16),

    /** Display of the DRM content has been paused.
     *  @note This request is currently not implemented and will not be sent
     *  by Picsel. */
    PicselUserRequest_DrmInfo_ConsumePause,

    /** Display of the DRM content has been Resumed.
     *  @note This request is currently not implemented and will not be sent
     *  by Picsel. */
    PicselUserRequest_DrmInfo_ConsumeResume,

    /** The DRM content has stopped being displayed. */
    PicselUserRequest_DrmInfo_ConsumeStop
}
PicselUserRequest_DrmInfo_ConsumptionEvent;

/**
 * @ingroup TgvDrmFiles
 * Validation result for DRM access.
 */
typedef enum PicselUserRequest_DrmInfo_ValidationResult
{
    /** Access to the content is allowed. */
    PicselUserRequest_DrmInfo_Valid  = (1<<16),

    /** Access to the content is denied. */
    PicselUserRequest_DrmInfo_Invalid
}
PicselUserRequest_DrmInfo_ValidationResult;

/**
 * @ingroup TgvDrmFiles
 * Identifies the type of DRM notification that will be presented to the end
 * user by Picsel when DRM content is about to display.  The notification
 * will be presented as a pop-up dialog, containing the drmInfo string
 * provided by the Alien application.
 */
typedef enum PicselUserRequest_DrmInfo_PopUpType
{
    /** No pop up will be displayed - the DRM protected content will just
     *  display. */
    PicselUserRequest_DrmInfo_NoPopUp     = (1<<16),

    /** A pop up will be displayed showing the drmInfo string, and
     *  presenting the end user with a yes/no choice of whether to display
     *  the DRM protected content or not. */
    PicselUserRequest_DrmInfo_ChoicePopUp,

    /** A pop up will be displayed showing the drmInfo string.  Once the
     *  user dismisses the dialog, the DRM content will display. */
    PicselUserRequest_DrmInfo_NoticePopUp
}
PicselUserRequest_DrmInfo_PopUpType;

/**
 * @ingroup TgvDrmFiles
 * Consumption data result for a DRM request.  This will be sent to the Alien
 * application in a PicselUserRequest_DrmInfo request when the content starts
 * or stops being displayed.  The Alien application must report whether the
 * consumption was successful, i.e. whether after consuming any limited
 * rights, the application is still licensed to display the content.
 */
typedef struct PicselUserRequest_DrmInfo_ConsumptionData
{
    /** [in] The type of consumption event. */
    PicselUserRequest_DrmInfo_ConsumptionEvent event;

    /** [out] 1 if after consuming DRM rights the DRM protected content can
    *         still be displayed, or 0 if the rights to display it have
    *         all been consumed. */
    int                                        allowed;
}
PicselUserRequest_DrmInfo_ConsumptionData;

/**
 * @ingroup TgvDrmFiles
 *  Validation data of DRM content
 */
typedef struct PicselUserRequest_DrmInfo_ValidationData
{
    /** [out] The validation result for the DRM content. */
    PicselUserRequest_DrmInfo_ValidationResult result;

    /** [out] The type of notification popup that the Picsel library must
     *        show before displaying the DRM protected content. */
    PicselUserRequest_DrmInfo_PopUpType        popUpType;

    /** [out] A UTF-8 DRM information string that will be displayed in the
     *        DRM notification popup.  A suggested use for this string is to
     *        notify the end user of the extent of the remaining DRM access
     *        rights for the protected content, e.g. remaining number of
     *        plays, or remaining license time. It is valid for this string
     *        to be NULL only if the popup is of type
     *        PicselUserRequest_DrmInfo_NoPopUp.
     */
    const char*                                drmInfo;
}
PicselUserRequest_DrmInfo_ValidationData;

/**
 * @ingroup TgvDrmFiles
 * All DRM requests use this request type to validate access to DRM protected
 * content.  The Alien application must provide information about whether
 * access to the content is allowed or not.
 */
typedef struct PicselUserRequest_DrmInfo
{
    /** [in] UTF-8 URL string identifying the DRM protected content that is
     *       being accessed. */
    const char*                                    url;

    /** [in] The type of request being performed. */
    PicselUserRequest_DrmInfo_OperationType        opType;
    union
    {
        /** If opType is PicselUserRequest_DrmInfo_Validation, then data
         *  will be of this type. */
        PicselUserRequest_DrmInfo_ConsumptionData  consumptionData;

        /** If opType is PicselUserRequest_DrmInfo_Consumption, then data
         *  will be of this type. */
        PicselUserRequest_DrmInfo_ValidationData   validationData;
    }
    data;
}
PicselUserRequest_DrmInfo;

/**
 *  The format category of an Excel cell
 */
typedef enum ExcelCellType
{
    ExcelCellType_General = 0x10000,
    ExcelCellType_Text,
    ExcelCellType_Number,
    ExcelCellType_Fraction,
    ExcelCellType_Percentage,
    ExcelCellType_Scientific,
    ExcelCellType_Currency,
    ExcelCellType_Accounting,
    ExcelCellType_Date,
    ExcelCellType_Time,
    ExcelCellType_Custom
}
ExcelCellType;


/**
 *  A category of excel formulae e.g. Statistical
 */
typedef struct FormulaeCategory
{
    /**
     * [in]     Title of category
     */
    const char  *title;
    /**
     * [in]     Array of formula strings
     */
    const char **formulae;
    /**
     * [in]     Array of corresponding description strings
     */
    const char **descriptions;
    /** [in]    Size of arrays of formula strings and corresponding descriptions
     */
    int         formulaeLength;
}
FormulaeCategory;

/**
 * Settings to be applied to an excel cell edit dialogue box.
 *
 * This struct should be sent along with a @ref PicselUserRequest_Type_ExcelCellEdit
 * request.
 */
typedef struct PicselUserRequest_ExcelCellEdit
{
    /**
     * [in] The default contents of the dialogue input field.
     */
    const char *stringDefault;
    /**
     * [in]     Text to be displayed in the request.
     */
    const char *caption;
    /** [in]    Maximum allowed input characters.
     *          If zero (0) is specified, then there will be no limit on the
     *          number of input characters.
     */
    int         maxChar;
    /** [in]    boolean flag:
     *
     *          1 - entry may include newlines.
     *          0 - entry may not include newlines.
     */
    int         multiline;
    /**
     *  [out]   User's response, as entered into the text field
     */
    char       *string;
    /**
     * [in]     dialogue name:  allows the named instance of the dialogue to
     *          override the default behaviour using an alien-defined override
     *          mechanism. Set this to NULL if it is not used.
     */
    const char *name;
    /**
     * [in]    Array of excel formula categories
     */
    FormulaeCategory *categories;
    /**
     * [in]    Size of array of excel formula categories
     */
    int         numberOfCategories;
    /**
     * [in]    The format category of the selected Excel cell
     */
    ExcelCellType cellType;
}
PicselUserRequest_ExcelCellEdit;

/**
 * Data for the host to process the printing request from the application
 *
 * This struct should be sent along with a @ref PicselUserRequest_Type_PrintDocument
 * request.
 */
typedef struct PicselUserRequest_PrintDocument
{
    Picsel_View           *picselView;  /**< [in]  NULL, reserved for future
                                         *         use */
    unsigned int          *pages;       /**< [in]  Array of page numbers to
                                         *         be printed.  Page numbers
                                         *         start at 1.  If NULL, all
                                         *         pages in the document
                                         *         should be printed */
    unsigned int           numPages;    /**< [in]  The number of page numbers
                                         *         in the array */
    unsigned int           numCopies;   /**< [in]  The number of copies
                                         *         required */
    char                  *identifier;  /**< [in]  The identifier of the
                                         *         printer to be used, or
                                         *         NULL if any available
                                         *         printer should be used */
    unsigned char          colour;      /**< [in]  Indicates whether or not
                                         *         the printout should be
                                         *         produced in colour.
                                         *         0 == mono, 1 == colour */
    PicselPrint_Emulation  emulation;   /**< [in]  The emulation to be used */
    unsigned int           quality;     /**< [in]  The quality of print output
                                         *         (in dots per inch) */
    char                  *message;     /**< [out] In the event of a failure,
                                         *         the alien may fill this in
                                         *         with a message to display
                                         *         to the user to indicate the
                                         *         problem that occurred, or
                                         *         NULL if this message is
                                         *         not provided */
}
PicselUserRequest_PrintDocument;

/**
 * Information about a printer
 */
typedef struct PicselPrint_PrinterInfo
{
    char                  *identifier;  /**< A unique identifier for this
                                         *   printer on this device.  For
                                         *   example, this could be a port,
                                         *   IP address, or ID */
    char                  *name;        /**< The printer's name.  The Picsel
                                         *   library may present this to the
                                         *   user, so where possible this
                                         *   should be localised
                                         *   appropriately */
    char                  *description; /**< The printer's description.  The
                                         *   Picsel library may present this
                                         *   to the user, so where possible
                                         *   this should be localised
                                         *   appropriately */
    PicselPrint_Emulation  emulation;   /**< The printer emulation modes
                                         *   supported by the printer.
                                         *   Multiple values may be bitwise
                                         *   OR'd (|) together */
    unsigned char          colour;      /**< Indicates whether or not colour
                                         *   printing is supported.
                                         *   0 == mono, 1 == colour */
}
PicselPrint_PrinterInfo;

/**
 * Data structure sent along with a @ref PicselUserRequest_Type_SearchForPrinters
 * request.
 */
typedef struct PicselUserRequest_SearchForPrinters
{
    Picsel_View             *picselView;  /**< [in]  NULL, reserved for
                                           *         future use */
    const char              *identifier;  /**< [in]  If non-NULL, only
                                           *         details for the printer
                                           *         with the specified
                                           *         identifier should be
                                           *         returned.  If NULL,
                                           *         all available printers
                                           *         should be returned */
    PicselPrint_PrinterInfo *printers;    /**< [out] Array of printers found */
    unsigned int             numPrinters; /**< [out] Number of entries in the
                                           *         printers array */
}
PicselUserRequest_SearchForPrinters;

/**
 * File Upload type
 */
typedef enum PicselUploadDocument_Type
{
    PicselUploadDocument_Type_Save      = 0, /**< Document should be uploaded with
                                              *   its existing filename */
    PicselUploadDocument_Type_SaveAs    = 1, /**< The user should be prompted for
                                              *   the filename used to save the
                                              *   document */
    PicselUploadDocument_Type_SaveLocal = 2  /**< Document should moved to
                                              *   application-specific local
                                              *   storage */
} PicselUploadDocument_Type;

/**
 * Data structure sent along with a @ref PicselUserRequest_Type_UploadDocument
 * request.
 */
typedef struct PicselUserRequest_UploadDocument
{
    Picsel_View                *picselView;       /**< [in]  NULL, reserved for
                                                   *         future use */
    const char                 *fileNameOriginal; /**< [in] the name of the file
                                                   *        that was originally
                                                   *        opened. */
    const char                 *fileNameTemp;     /**< [in] the temporary file to
                                                   *        be uploaded to the
                                                   *        cloud. */
    const char                 *mime;             /**< [in] the MIME type of the file */
    PicselUploadDocument_Type   type;             /**< [in] save or save as */
    char                       *fileNameNew;      /**< [out] the updated file name
                                                             for save as */
}
PicselUserRequest_UploadDocument;

typedef enum MessageEditType
{
    /** Message edit type to specifiy that the application has no preference
     *  as to the message type */
    MessageEdit_Type_Unspecified = (1<<16),

    /** Message type to specify that the application wishes to send an SMS */
    MessageEdit_Type_SMS,

    /** Message type to specify that the application wishes to send an MMS */
    MessageEdit_Type_MMS,

    /** Message type to specify that the application wishes to send an
     *  email */
    MessageEdit_Type_Email
}
MessageEditType;

/**
 * Data for the host to process the message editing request from the
 * application
 *
 * This struct should be sent along with a @ref PicselUserRequest_Type_MessageEdit
 * request.
 */
typedef struct PicselUserRequest_MessageEdit
{
    /** [in] Type of message to be sent */
    MessageEditType type;

    /** [in] The contents of the From: field of the message, or NULL if none
     *       specified */
    const char     *from;

    /** [in] The contents of the To: field of the message, or NULL if none
     *       specified */
    const char     *to;

    /** [in] The contents of the Cc: field of the message, or NULL if none
     *       specified */
    const char     *cc;

    /** [in] The contents of the Bcc: field of the message, or NULL if none
     *       specified */
    const char     *bcc;

    /** [in] The contents of the Subject: field of the message, or NULL if none
     *       specified */
    const char     *subject;

    /** [in] The contents of the message itself, or NULL if none specified */
    const char     *message;

    /** [in] The number of attachments in the message */
    unsigned int    attachmentCount;

    /** [in] Array of length @c attachmentCount, with a list of filenames
     *       containing the location of the attachments, or NULL if
     *       @c attachmentCount is 0. Note that the filenames will only
     *       remain valid for the duration of the user request, and will
     *       be removed once PicselUserRequest_notify is called. The alien
     *       should take a copy of the files itself if they are required
     *       for a longer duration. */
    const char    **attachments;

    /** [in] Array of length @c attachmentCount, with a list of MIME types
     *       corresponding to the attachments in @c attachments, or NULL if
     *       @c attachmentCount is 0 */
    const char    **mimetypes;

    /** [out] In the event of a failure, the alien may fill this in
              with a message to display to the user to indicate the
              problem that occurred, or NULL if this message is not
              provided */
    char           *resultMessage;
}
PicselUserRequest_MessageEdit;

/**
 * Data for the host to process the document sharing request from the
 * application
 *
 * This struct should be sent along with a @ref PicselUserRequest_Type_ShareDocument
 * request.
 */
typedef struct PicselUserRequest_ShareDocument
{
    const char *document;   /**< [in] the full path to the document to be shared */
}
PicselUserRequest_ShareDocument;

/* ****************************** Common Data ******************************* */


/**
 * @ingroup TgvContentRequests
 * A union of all the possible user requests that may occur.
 */
typedef union PicselUserRequest_Data
{
    PicselUserRequest_Authentication     authentication;
    PicselUserRequest_Password           password;
    PicselUserRequest_Notify             notify;
    PicselUserRequest_Query              query;
    PicselUserRequest_String             string;
    PicselUserRequest_GetClipboard       getClipboard;
    PicselUserRequest_GetImage           getImage;
    PicselUserRequest_GetSavePath        getSavePath;
    PicselUserRequest_Cookie             cookie;
    PicselUserRequest_HttpReferrer       httpReferrer;
    PicselUserRequest_Certificate        certificate;
    /** Response given in 'response' member of request data instead
      * of 'result' member of the main request */
    PicselUserRequest_Transition         transition;
    PicselUserRequest_Redirect           redirect;
    PicselUserRequest_InsecureObjects    insecureObjects;
    PicselUserRequest_ChooseFileName     fileSelect;
    PicselUserRequest_ValidateFileName   fileValidate;
    PicselUserRequest_AntiVirus_URL      antiVirusUrl;
    PicselUserRequest_AntiVirus_Content  antiVirusContent;
    PicselUserRequest_FormAutoComplete   formAutoComplete;
    PicselUserRequest_DownloadConfirm    downloadConfirm;
    PicselUserRequest_DownloadData       downloadData;
    PicselUserRequest_ContactsOperation  contactsOperation;
    PicselUserRequest_DateTime           dateTime;
    PicselUserRequest_FolderOpen         folderOpen;
    PicselUserRequest_SecretModeChange   secretModeChange;
    PicselUserRequest_CreateBookmark     createBookmark;
    PicselUserRequest_SearchNextPage     searchNextPage;
    PicselUserRequest_DrmInfo            drmInfo;
    PicselUserRequest_ExcelCellEdit      excelCellEdit;
    PicselUserRequest_PrintDocument      printDocument;
    PicselUserRequest_MessageEdit        messageEdit;
    PicselUserRequest_SearchForPrinters  searchForPrinters;
    PicselUserRequest_UploadDocument     uploadDocument;
    PicselUserRequest_ShareDocument      shareDocument;
}
PicselUserRequest_Data;


/*
 * @ingroup TgvContentRequests
 * Private data for Picsel
 */
typedef struct PicselUserRequest_Internal PicselUserRequest_Internal;


/**
 * @ingroup TgvContentRequests
 *
 * Indicates which option was chosen; either by the user in response to a prompt
 * , or by the Alien Application through a policy-based decision. See @ref
 * TgvContentRequests.
 */
typedef enum PicselUserRequest_Result
{
    /**
     * The "Yes" (OK) option was chosen by the user.
     */
    PicselUserRequest_Result_Accepted = (1<<16),
    /**
     * The "No" (Cancel) option was chosen by the user. (This does not imply
     * that the dialog box need have a Reject option unless stated in the
     * request's description).
     */
    PicselUserRequest_Result_Rejected,
    /**
     * An error occurred. Picsel recommends that the Alien Application behaves
     * as if the request was rejected, optionally notifying the device user that
     * an error occurred.
     */
    PicselUserRequest_Result_Error
}
PicselUserRequest_Result;

/**
 * @ingroup TgvContentRequests
 *
 * Container for requests to the user for information.
 *
 * Each fields' meaning is described in its structure definition. Only fields
 * marked [out] may be updated by the Alien Application. Picsel will read the
 * contents of fields marked [out] during  PicselUserRequest_notify(), but
 * will not modify the fields. Alien may free the contents immediately
 * afterwards.
 *
 * Picsel will only set a string to NULL if it is not needed in the current
 * context. For example the caption in a @ref PicselUserRequest_Type_Query
 * request will not be NULL, but could be "" if there is no default
 * to be provided.
 *
 * The Alien application may set a field to either NULL or "" to
 * indicate that the field is empty.
 */
typedef struct PicselUserRequest_Request
{

    /** [in] Internal data to Picsel. Do not use. */
    PicselUserRequest_Internal *internal;

    Picsel_View                *picselView; /**< [in] NULL, reserved for future use   */

    PicselUserRequest_Type      type; /**< [in]  Type of request    */

    PicselUserRequest_Data     *requestData;/**< [out] Data for request     */
    PicselUserRequest_Result    result;     /**< [out] result of request    */
}
PicselUserRequest_Request;


/**
 * @ingroup TgvContentRequests
 * Issue a request to the user for a response.
 *
 * This function must return immediately. Once the user has responded to the
 * input, PicselUserRequest_notify() should be called.
 *
 * If the request cannot be queued, or it cannot be completed, then the Alien
 * Application must set @ref PicselUserRequest_Request->result to @ref
 * PicselUserRequest_Result_Error and then call PicselUserRequest_notify().
 *
 * The Alien Application should copy any strings pointed to within the union
 * pointed to by @c request, as they will be deallocated when this function
 * returns. It is recommended to retain a pointer to @c request, until the Alien
 * Application has callled PicselUserRequest_notify() has been called with the
 * response to the request.
 *
 * @param  alienContext See PicselApp_start().
 * @param  request      The data describing the question to ask the user or
 *                      policy.
 */
void AlienUserRequest_request(Alien_Context              *alienContext,
                              PicselUserRequest_Request  *request);


/**
 * @ingroup TgvContentRequests
 * Terminate a currently active request.
 *
 * This function must return immediately, and the Alien Application should not
 * call PicselUserRequest_notify(); the request is no longer valid.
 *
 * @param  alienContext See PicselApp_start().
 * @param  request      The @c request previously sent to
 *                      AlienUserRequest_request(), a response to which is no
 *                      longer required.
 */
void AlienUserRequest_requestTerminate(Alien_Context             *alienContext,
                                       PicselUserRequest_Request *request);


/**
 * @ingroup TgvContentRequests
 * Notify the Picsel Library of the result of a request, received earlier via
 * AlienUserRequest_request(). This function should be called from the thread
 * the Picsel Library is running in, but not from a Picsel callback.
 *
 * @pre Call this function in the thread the Picsel library is running, but
 * not from a Picsel callback.
 *
 * @post @c picselInputRequest will be freed.  Do not access any of the
 * fields of this struct after this function returns.  Data pointed to by
 * [out] fields in the @ref PicselUserRequest_Request.requestData will not
 * be freed. Pointers to these fields must be retained and freed after this
 * function returns. See "Memory and data conservation" above.
 *
 * @param picselContext       Set by AlienEvent_setPicselContext()
 * @param picselInputRequest  A pointer to the input request, as provided to
 *                            AlienUserRequest_request().
 *
 * @return                    1 if called in response to
 *                            AlienUserRequest_request().  Return value is
 *                            undefined if called at other times.
 */
int PicselUserRequest_notify(Picsel_Context              *picselContext,
                             PicselUserRequest_Request   *picselInputRequest);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !ALIEN_REQUEST_H */
