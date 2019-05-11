/**
 * Error reporting by Picsel library to the Alien application
 *
 * The functions in this file must be implemented by the Alien application
 * before linking with the TGV library.
 *
 * @file
 * $Id: alien-error.h,v 1.36 2012/11/29 09:44:37 hugh Exp $
 */
/* Copyright (C) Picsel, 2005-2008. All Rights Reserved. */
/**
 * @defgroup TgvError Error Reporting
 * @ingroup TgvNotify
 *
 * The list of errors originated by the Picsel library. They notify
 * the Alien application about possible problems that may happen within
 * the Picsel library. Errors are different from the other information
 * notifications (see @ref TgvContentInformation) because they indicate
 * that the requested operation has not been completed fully. However,
 * this may be for good reasons, such as that a document chosen by
 * the user was corrupt.
 *
 * These errors must be handled in the Alien application since they
 * may affect the application behavior. Not all of these errors are fatal,
 * most of them are warnings which may be ignored. The Alien Application
 * Developer should decide if the error may be ignored or the user's
 * interaction is required in order to handle the error. Error handler
 * functions defined in this group must be implemented in the Alien
 * application.
 *
 * @{
 */

#ifndef ALIEN_ERROR_H
#define ALIEN_ERROR_H

#include "alien-types.h"
#include "alien-legacy.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** List of errors returned by Picsel library
 */
typedef enum PicselError
{
    /** The Picsel library cannot load the requested file type. After
     * the Alien application called PicselApp_loadDocument() the Picsel
     * library tried to find a suitable Document Agent to interpret
     * the file content and on this occasion, it failed. This matching
     * is normally done using the file name or MIME type. A new document
     * will not be displayed, the screen will keep the previous state.
     * The Alien application may wish to notify the user, prompt them
     * to open another file, or take other action.
     * Also see @ref TgvInitialiseAgents().
     */
    PicselDocumentError_AgentMatchFailed = (1<<16),

    /** Picsel library failed to find a suitable Document Agent because
     * no data was available to recognize the document content. This error
     * is similar to @ref PicselDocumentError_AgentMatchFailed. The difference
     * is that Picsel library returns this error if it failed to find any
     * valid data in the file. This may happen in two cases:
     * - all Document Agents failed to recognize the file so returned an error,
     * - the file contains no data.
     *
     * This is not a fatal error so the Picsel library will not be terminated.
     */
    PicselDocumentError_AgentMatchFailedNoData,

    /** Picsel library failed to decode part of the document content. This
     * error is generated by the Document Agent and may occur when the file
     * contains formatting unsupported by Picsel product, or when the file
     * data has been corrupted or truncated. Part of the document may be
     * available for viewing if @ref AlienInformation_DocumentContentAvailable
     * has already been notified but it may be wrongly formatted or content
     * may be missed. The Alien application may wish to notify the user of
     * this.
     */
    PicselDocumentError_DocumentTranslationFailed,

    /** The Picsel library has malfunctioned. This may occur when the Picsel
     * library encountered an unexpected internal problem which is not covered
     * by the other @ref PicselError errors. This error is not fatal so
     * the Picsel library will continue to run. The Alien application may
     * wish to notify the user of this.
     */
    PicselDocumentError_InternalError,

    /** An unsuccessfull attempt to load a password protected file with
     * unsupported encryption. This error is returned if the document
     * contains encrypted data which cannot be parsed by a Document
     * Agent. It may happen if the encryption support for this document
     * type is not available in Picsel library. It may be appropriate
     * to display an error message or prompt the user to open another file.
     * A new document will not be displayed, the screen will keep
     * the previous state.
     *
     * Encryption is not supported for .XLS, .PPT and .DOC documents.
     * This is a premium feature for .ZIP and .PDF files which is not
     * included in standard Picsel libraries. Please check the project
     * specification or contact your Picsel support representative
     * to confirm its availability.
     *
     * This error is not returned if an incorrect password is submitted to
     * the Picsel library on @ref PicselUserRequest_Type_Document_Password
     * request.
     */
    PicselDocumentError_DocumentPasswordProtected,

    /**
     * The default character encoding set by @ref PicselConfig_defaultCharSet
     * is not available, and the document has not specified an encoding. This
     * indicates a bug in the Alien application; either an invalid default
     * has been set, or the appropriate language features have not been
     * registered, for example Picsel_Encoding_Register_zhcn(). The current
     * document will be displayed using ISO-8859-1.
     * It is recommended to ignore this error.
     */
    PicselDocumentError_UnsupportedCharset,

    /** The Picsel application could not complete a previously requested
     * operation because of insufficient device memory.
     *
     * This is a warning, not an error. The Picsel library can continue to
     * run, and there is no need to take action. It may be appropriate to
     * display a warning message to the user, then continue.
     *
     * This event will be sent after every memory allocation failure, so
     * may be sent many times in response to a single user action. The
     * Alien Application Developer is responsible for limiting the number
     * of notifications to the user.
     *
     * If there is no meaningful content to display, other events will be
     * sent, including @ref AlienInformation_InitFailed or
     * @ref PicselLoadedStatus_NotLoaded depending on the circumstances.
     * If @ref AlienInformation_DocumentContentAvailable has been sent, then the
     * application is advised to continue to run normally.
     */
    PicselError_OutOfMemory,

    /** The settings path is undefined. The Alien application made an attempt
     * to activate a feature that requires the property @ref PicselConfig_settingsPath
     * to be defined. This error reports a problem in the Alien application
     * which should be modified to set the correct paths for the location
     * of the persistent settings ( history, bookmarks, cookies). It is
     * recommended not to notify the user about this error but the Alien
     * Application Developer may wish to report it during the Alien application
     * testing, so that the Alien application can be fixed.
     */
    PicselError_SettingsPathUndefined,

    /** The current web page contains JavaScript which attempted to open
     * another pop-up window, but this is not supported by the Picsel library.
     * It is not a fatal error so the Picsel library will continue to run.
     * The Alien application may wish to notify the user that the pop up window
     * will not be displayed, although it is recommended to ignore this error.
     * This error can be returned only if the JavaScript feature is enabled
     * in the Picsel library.
     */
    PicselError_UnsupportedJSPopup,

    /** This error is not used by Picsel at the moment and reserved for future
     * products which may report that the file selected in a form on a web page
     * could not be uploaded to the web server because it is larger than
     * the allowed size.
     * @product Browser.
     */
    PicselError_UploadFileTooLarge,

    /** This error can not be returned by the Picsel library and is defined for
     * a type validation. Ensure this is always the last error. */
    PicselError_LastErrorUnused

}
PicselError;


/**
 * Background information on a @ref PicselError cause.
 */
typedef enum PicselErrorCause
{
    PicselErrorCause_Unknown = (1<<16), /**< Indicates that there is no other
                                             known cause for an error */
    PicselErrorCause_OutOfMemory        /**< Indicates that the error was
                                             caused by memory allocation
                                             failure */
}
PicselErrorCause;

/**
 * Error data
 *
 * Structure used to pass any additional information that is available for an
 * error.
 */
typedef struct PicselErrorData
{
    /** This field is reserved for a future expansion. It indentifies
     * the view (document window) in which the error occured.
     */
    Picsel_View      *picselView;

    /** Additional information about the cause of an error,
     * if the information is available.
     */
    PicselErrorCause  errorCause;
}
PicselErrorData;

/**
 * Receive notification of an error from Picsel library.
 *
 * This function is called by Picsel library whenever an error is detected
 * within the library. The function must be implemented in the Alien Application.
 * The error code which is passed to the function should be handled depending
 * on the error nature and its severity. Responses to different errors
 * varies, the Alien Application Developer should look at the documenation for
 * each particular error.
 *
 * @param[in] alienContext  See PicselApp_start().
 * @param[in] error         The error code. See @ref PicselError.
 * @param[in] errorData     This field contains additional information on
 *                          the error cause. It can say if the error was
 *                          caused by the device running out of memory.
 *                          This field may be NULL. See @ref PicselErrorData.
 *
 * @runtime It is important that this function should return promptly as
 * the Picsel library will block until it does so.
 *
 * @implement It is recommended to use async dialogs to avoid blocking
 * the application.
 *
 * Example:
 * @code
 *     void AlienError_error(Alien_Context   *alienContext,
 *                       PicselError      error,
 *                       PicselErrorData *errorData)
 *     {
 *         switch(error)
 *         {
 *             case PicselDocumentError_AgentMatchFailed:
 *             case PicselDocumentError_AgentMatchFailedNoData:
 *                 displayDialog("Document cannot be displayed");
 *             break;
 *
 *             case PicselDocumentError_DocumentTranslationFailed:
 *                 displayDialog("Document may be corrupted "
 *                               "therefore cannot be fully displayed");
 *             break;
 *
 *             case PicselError_UnsupportedJSPopup:
 *                 // ignore this error
 *             break;
 *
 *              ...
 *
 *         }
 *    }
 * @endcode
 *
 */
void AlienError_error(Alien_Context   *alienContext,
                      PicselError      error,
                      PicselErrorData *errorData);

/**
 * Receive notification of a fatal error from Picsel library.
 *
 * This function is called instead of AlienError_error() whenever a fatal error
 * is detected within Picsel library which is none of @ref PicselError errors.
 * The error may be raised when the device runs out of memory but in this case
 * it caused the application to fail to start up or continue running
 * on the device. This function must be implemented in the Alien Application.
 * When this function returns, Picsel library will be shutdown and
 * will not process any events until the Picsel library is re-started. Picsel
 * library must not be called from this function.
 *
 * @param[in] alienContext  See PicselApp_start()
 *
 * @post Picsel library must not be called after this function has been called,
 * except PicselApp_start().
 *
 * @implement The Picsel library may already have shut down partially, and so
 * the Alien application must not call any Picsel functions, for example
 * @ref PicselControl_getViewState().
 *
 * Example:
 *
 * @code
 *    void AlienError_fatal(Alien_Context *alienContext)
 *    {
 *        displayMessage( "Fatal error: application has shutdown");
 *        releaseResources( alienContext);
 *        shutdownApp( alienContext);
 *    }
 * @endcode
 */
void AlienError_fatal(Alien_Context *alienContext);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !ALIEN_ERROR_H */
