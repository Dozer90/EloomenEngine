#pragma once

#include "error_codes/filesystem_errc.h"

#include "utility/defines.h"
#include "utility/bitflag_helpers.h"

#include "streaming/streaming.h"

#include <EASTL/span.h>
#include <EASTL/vector.h>
#include <EASTL/functional.h>
#include <EASTL/memory.h>
#include <EASTL/chrono.h>
#include <EASTL/string.h>
#include <EASTL/optional.h>
#include <EASTL/variant.h>

#include <filesystem>


namespace eloo::filesystem {

    namespace directory { ELOO_DECLARE_ID_T; }
    namespace file      { ELOO_DECLARE_ID_T; }

    using path_t = std::filesystem::path;
    using path_list_t = eastl::vector<path_t>;
    using file_time_t = eastl::chrono::milliseconds;
    using search_predicate = eastl::function<bool(const path_t&)>;

    using perms = std::filesystem::perms;
    using copy_opts = std::filesystem::copy_options;

    using void_r            = ELOO_DECLARE_RESULT_TYPE(void);
    using file_id_r         = ELOO_DECLARE_RESULT_TYPE(file::id_t);
    using directory_id_r    = ELOO_DECLARE_RESULT_TYPE(directory::id_t);
    using directory_r       = ELOO_DECLARE_RESULT_TYPE(path_list_t);
    using bool_r            = ELOO_DECLARE_RESULT_TYPE(bool);
    using path_r            = ELOO_DECLARE_RESULT_TYPE(path_t);
    using path_list_r       = ELOO_DECLARE_RESULT_TYPE(path_list_t);
    using file_time_r       = ELOO_DECLARE_RESULT_TYPE(file_time_t);
    using perms_r           = ELOO_DECLARE_RESULT_TYPE(perms);
    using size_r            = ELOO_DECLARE_RESULT_TYPE(eastl_size_t);


    // ==============================================================================
    // General management

    //$ Create a directory or file at the specified path.
    //$ arg0:   path_t path => The new path of the directory/file (including extension). Relative paths will be resolved against the current working directory.
    //$ arg2:   bool overwriteFileIfExists [false] => If false and creating a file, returns an error if the file already exists.
    //$ return: void_r => The result of the creation operation.
    void_r create(path_t path, bool overwriteFileIfExists = false);

    //$ Create a directory at the specified path and register it in the filesystem.
    //$ arg0:   path_t targetPath => The absolute path of the directory to create.
    //$ return: directory_id_r => The ID of the created directory or an error code.
    directory_id_r create_and_register_directory(path_t targetPath);

    //$ Create a file at the specified path and register it in the filesystem.
    //$ arg0:   path_t targetPath => The absolute path of the file to create.
    //$ arg1:   bool overwriteFileIfExists [false] => If false, returns an error if the file already exists.
    //$ return: file_id_r => The ID of the created file or an error code.
    file_id_r create_and_register_file(path_t targetPath, bool overwriteFileIfExists = false);


    //$ Delete a directory or file at the specified path.
    //$ arg0:   path_t targetPath => The absolute path of the directory or file to delete.
    //$ return: void_r => The result of the delete operation.
    //$ note:   This will delete the directory or file permanently! Use with caution.
    void_r destroy(path_t targetPath);

    //$ Delete a directory by its ID.
    //$ arg0:   directory::id_t targetID => The ID of the directory to delete.
    //$ return: void_r => The result of the delete operation.
    //$ note:   This will delete the directory permanently! Use with caution.
    void_r destroy(directory::id_t targetID);

    //$ Delete a file by its ID.
    //$ arg0:   file::id_t targetID => The ID of the file to delete.
    //$ return: void_r => The result of the delete operation.
    //$ note:   This will delete the file permanently! Use with caution.
    void_r destroy(file::id_t targetID);


    //$ Clears the contents of a directory.
    //$ arg0:   path_t targetPath => The absolute path of the directory to clear.
    //$ return: void_r => The result of the clear operation.
    //$ note:   This will delete all contents of the directory but keep the directory itself intact.
    void_r clear_directory(path_t targetPath);

    //$ Clears the contents of a directory by its ID.
    //$ arg0:   directory::id_t targetID => The ID of the directory to clear.
    //$ return: void_r => The result of the clear operation.
    //$ note:   This will delete all contents of the directory but keep the directory itself intact.
    void_r clear_directory(directory::id_t targetID);


    //$ Rename a directory or file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to rename.
    //$ arg1:   eastl::wstring_view newName => The new name for the directory or file.
    //$ return: void_r => The result of the rename operation.
    void_r rename(path_t objectPath, const path_t& newName);

    //$ Rename a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to rename.
    //$ arg1:   eastl::wstring_view newName => The new directory name.
    //$ return: void_r => The result of the rename operation.
    void_r rename(directory::id_t dirID, const path_t& newName);

    //$ Rename a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to rename.
    //$ arg1:   eastl::wstring_view newName => The new file name, including extension.
    //$ return: void_r => The result of the rename operation.
    void_r rename(file::id_t fileID, const path_t& newName);


    //$ Copy a directory or file to a new location.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to copy.
    //$ arg1:   path_t destinationPath => The absolute path where the directory or file should be copied to.
    //$ return: void_r => The result of the copy operation.
    void_r copy(path_t objectPath, path_t destinationPath, copy_opts opts = copy_opts::none);

    //$ Copy a directory by its ID to a new location.
    //$ arg0:   directory::id_t dirID => The ID of the directory to copy.
    //$ arg1:   path_t destinationPath => The absolute path where the directory should be copied to.
    //$ return: void_r => The result of the copy operation.
    void_r copy(directory::id_t dirID, path_t destinationPath, copy_opts opts = copy_opts::none);

    //$ Copy a file by its ID to a new location.
    //$ arg0:   file::id_t fileID => The ID of the file to copy.
    //$ arg1:   path_t destinationPath => The absolute path where the file should be copied to.
    //$ return: void_r => The result of the copy operation.
    void_r copy(file::id_t fileID, path_t destinationPath, copy_opts opts = copy_opts::none);


    //$ Move a directory or file to a new location.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to move.
    //$ arg1:   path_t destinationPath => The absolute path where the directory or file should be moved to.
    //$ return: void_r => The result of the move operation.
    void_r move(path_t objectPath, path_t destinationPath);

    //$ Move a directory by its ID to a new location.
    //$ arg0:   directory::id_t dirID => The ID of the directory to move.
    //$ arg1:   path_t destinationPath => The absolute path where the directory should be moved to.
    //$ return: void_r => The result of the move operation.
    void_r move(directory::id_t dirID, path_t destinationPath);

    //$ Move a file by its ID to a new location.
    //$ arg0:   file::id_t fileID => The ID of the file to move.
    //$ arg1:   path_t destinationPath => The absolute path where the file should be moved to.
    //$ return: void_r => The result of the move operation.
    void_r move(file::id_t fileID, path_t destinationPath);


    //$ Set perms for a directory or file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to set perms for.
    //$ arg1:   perms perms => The perms to set for the directory or file.
    //$ return: void_r => The result of the set perms operation.
    void_r set_perms(path_t objectPath, perms perms);

    //$ Set perms for a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to set perms for.
    //$ arg1:   perms perms => The perms to set for the directory.
    //$ return: void_r => The result of the set perms operation.
    void_r set_perms(directory::id_t dirID, perms perms);

    //$ Set perms for a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to set perms for.
    //$ arg1:   perms perms => The perms to set for the file.
    //$ return: void_r => The result of the set perms operation.
    void_r set_perms(file::id_t fileID, perms perms);


    // ==============================================================================
    // Path helpers

    //$ Normalize slashes in a path to use the platform's preferred separator.
    //$ arg0:   path_t path => The path to normalize.
    //$ return: path_r => The normalized path with slashes adjusted.
    void normalize_path_slashes(path_t& path);


    //$ Canonicalize a path to its absolute form, resolving any relative components.
    //$ arg0:   path_t path => The path to canonicalize.
    //$ return: path_r => The canonicalized absolute path.
    path_r canonicalize_path(path_t path);


    bool resolve(path_t& path);
    bool resolve(path_t& path, directory::id_t dirID);


    // ==============================================================================
    // Working directory

    //$ Get the current working directory.
    //$ return: path => The current working directory as an absolute path.
    path_r get_working_directory() EA_NOEXCEPT;


    //$ Set the current working directory to the specified path.
    //$ arg0:   path_t targetPath => The absolute path of the directory to set as the working directory.
    //$ return: void_r => The result of the operation.
    void_r set_working_directory(path_t targetPath) EA_NOEXCEPT;

    //$ Set the current working directory to the specified directory ID.
    //$ arg0:   directory::id_t id => The ID of the directory to set as the working directory.
    //$ return: void_r => The result of the operation.
    void_r set_working_directory(directory::id_t id) EA_NOEXCEPT;


    // ==============================================================================
    // Registering directories

    //$ Register a directory at the specified path.
    //$ arg0:   path_t targetPath => The absolute path of the directory to register.
    //$ return: directory_id_r => The ID of the registered directory.
    directory_id_r register_directory(path_t targetPath);


    //$ Check if a directory is registered at the specified path.
    //$ arg0:   path_t targetPath => The absolute path of the directory to check.
    //$ return: bool_r => True if the directory is registered, false otherwise.
    bool_r is_directory_registered(path_t targetPath);


    //$ Deregister a directory by its ID.
    //$ arg0:   directory::id_t targetID => The ID of the directory to deregister.
    //$ return: void_r => The result of the deregistration operation.
    void_r deregister_directory(directory::id_t targetID);


    //$ Mark a subdirectory to be ignored when canonicalizing paths or listing contents.
    //$ arg0:   directory::id_t dirID => The ID of the directory to ignore the subdirectory in.
    //$ arg1:   path_t subDirPath => The relative path of the subdirectory to ignore.
    //$ return: void_r => The result of the ignore operation.
    void_r ignore_subdirectory(
        directory::id_t dirID,
        path_t subDirPath
    );


    //$ Check if a subdirectory is ignored in the specified directory.
    //$ arg0:   directory::id_t dirID => The ID of the directory to check for ignored subdirectories.
    //$ arg1:   path_t subDirPath => The relative path of the subdirectory to check.
    //$ return: bool_r => True if the subdirectory is ignored, false otherwise.
    bool_r is_subdirectory_ignored(
        directory::id_t dirID,
        path_t subDirPath
    );


    // ==============================================================================
    // Registering files

    //$ Register a file at the specified path.
    //$ arg0:   path_t targetPath => The absolute path of the file to register.
    //$ return: file_r => The ID of the registered file.
    file_id_r register_file(path_t targetPath);


    //$ Check if a file is registered at the specified path.
    //$ arg0:   path_t targetPath => The absolute path of the file to check.
    //$ return: bool_r => True if the file is registered, false otherwise.
    bool_r is_file_registered(path_t targetPath);

    //$ Check if a file is registered by its ID. Temporary IDs can be obtained from open_file_stream.
    //$ arg0:   file::id_t targetID => The ID of the file to check.
    //$ return: bool_r => True if the file is registered, false otherwise.
    bool_r is_file_registered(file::id_t targetID);


    //$ Deregister a file by its ID.
    //$ arg0:   file::id_t targetID => The ID of the file to deregister.
    //$ return: void_r => The result of the deregistration operation.
    void_r deregister_file(file::id_t targetID);


    // ==============================================================================
    // Checks

    //$ Check if a path points to a directory.
    //$ arg0:   path_t objectPath => The absolute path of the object to check.
    //$ return: bool_r => True if the path is a directory, false otherwise.
    bool_r is_directory(path_t objectPath);


    //$ Check if a path points to a file.
    //$ arg0:   path_t objectPath => The absolute path of the object to check.
    //$ return: bool_r => True if the path is a file, false otherwise.
    bool_r is_file(path_t objectPath);


    //$ Check if a path exists in the filesystem.
    //$ arg0:   path_t objectPath => The absolute path of the object to check.
    //$ return: bool_r => True if the path exists, false otherwise.
    bool_r exists(path_t objectPath);

    //$ Check if a directory exists by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to check.
    //$ return: bool_r => True if the directory exists, false otherwise.
    bool_r exists(directory::id_t dirID);

    //$ Check if a file exists by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to check.
    //$ return: bool_r => True if the file exists, false otherwise.
    bool_r exists(file::id_t fileID);


    //$ Get the perms of a directory or file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to get perms for.
    //$ return: perms_r => The perms of the directory or file.
    perms_r get_perms(path_t objectPath);

    //$ Get the perms of a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to get perms for.
    //$ return: perms_r => The perms of the directory.
    perms_r get_perms(directory::id_t dirID);

    //$ Get the perms of a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to get perms for.
    //$ return: perms_r => The perms of the file.
    perms_r get_perms(file::id_t fileID);


    //$ Check if a directory or file at the specified path is empty.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to check.
    //$ return: bool_r => True if the directory or file is empty, false otherwise.
    bool_r is_empty(path_t objectPath);

    //$ Check if a directory by its ID is empty.
    //$ arg0:   directory::id_t dirID => The ID of the directory to check.
    //$ return: bool_r => True if the directory is empty, false otherwise.
    bool_r is_empty(directory::id_t dirID);

    //$ Check if a file by its ID is empty.
    //$ arg0:   file::id_t fileID => The ID of the file to check.
    //$ return: bool_r => True if the file is empty, false otherwise.
    bool_r is_empty(file::id_t fileID);


    // ==============================================================================
    // File and directory information

    //$ Get the size of a directory or file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to get size for.
    //$ return: size_r => The size of the directory or file in bytes.
    size_r size(path_t objectPath);

    //$ Get the size of a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to get size for.
    //$ return: size_r => The size of the directory in bytes.
    size_r size(directory::id_t dirID);

    //$ Get the size of a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to get size for.
    //$ return: size_r => The size of the file in bytes.
    size_r size(file::id_t fileID);


    //$ Get the last edit time of a directory or file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to get last edit time for.
    //$ return: time_r => The last edit time as a duration in nanoseconds.
    file_time_r last_edit_time(path_t objectPath);

    //$ Get the last edit time of a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to get last edit time for.
    //$ return: time_r => The last edit time as a duration in nanoseconds.
    file_time_r last_edit_time(directory::id_t dirID);

    //$ Get the last edit time of a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to get last edit time for.
    //$ return: time_r => The last edit time as a duration in nanoseconds.
    file_time_r last_edit_time(file::id_t fileID);


    //$ Get the last access time of a directory or file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to get last access time for.
    //$ return: time_r => The last access time as a duration in nanoseconds.
    file_time_r last_access_time(path_t objectPath);

    //$ Get the last access time of a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to get last access time for.
    //$ return: time_r => The last access time as a duration in nanoseconds.
    file_time_r last_access_time(directory::id_t dirID);

    //$ Get the last access time of a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to get last access time for.
    //$ return: time_r => The last access time as a duration in nanoseconds.
    file_time_r last_access_time(file::id_t fileID);


    //$ Get the creation time of a directory or file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to get creation time for.
    //$ return: time_r => The creation time as a duration in nanoseconds.
    file_time_r creation_time(path_t objectPath);

    //$ Get the creation time of a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to get creation time for.
    //$ return: time_r => The creation time as a duration in nanoseconds.
    file_time_r creation_time(directory::id_t dirID);

    //$ Get the creation time of a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to get creation time for.
    //$ return: time_r => The creation time as a duration in nanoseconds.
    file_time_r creation_time(file::id_t fileID);


    //$ Get the absolute path of a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to get the absolute path for.
    //$ return: path_r => The absolute path of the directory.
    path_r absolute_path(directory::id_t dirID);

    //$ Get the absolute path of a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to get the absolute path for.
    //$ return: path_r => The absolute path of the file.
    path_r absolute_path(file::id_t fileID);


    //$ Get the extension of a file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the file to get the extension for.
    //$ arg1:   bool withDot => Whether to include the dot in the extension (default is true).
    //$ return: path_r => The file extension, including the dot if requested.
    path_r extension(
        path_t objectPath,
        bool withDot = true
    );

    //$ Get the extension of a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to get the extension for.
    //$ arg1:   bool withDot => Whether to include the dot in the extension (default is true).
    //$ return: path_r => The file extension, including the dot if requested.
    path_r extension(
        file::id_t fileID,
        bool withDot = true
    );


    //$ Get the name of a directory or file at the specified path.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to get the name for.
    //$ return: path_r => The name of the directory or file, including the extension.
    path_r name(path_t objectPath);

    //$ Get the name of a directory by its ID.
    //$ arg0:   directory::id_t dirID => The ID of the directory to get the name for.
    //$ return: path_r => The name of the directory.
    path_r name(directory::id_t dirID);

    //$ Get the name of a file by its ID.
    //$ arg0:   file::id_t fileID => The ID of the file to get the name for.
    //$ return: path_r => The name of the file, including the extension.
    path_r name(file::id_t fileID);


    //$ Get the name of a file at the specified path without its extension.
    //$ arg0:   path_t objectPath => The absolute path of the file to get the name without extension for.
    //$ return: path_r => The name of the file without its extension.
    path_r name_without_extension(path_t objectPath);

    //$ Get the name of a file by its ID without its extension.
    //$ arg0:   file::id_t fileID => The ID of the file to get the name without extension for.
    //$ return: path_r => The name of the file without its extension.
    path_r name_without_extension(file::id_t fileID);


    // ==============================================================================
    // Monitoring

    //$ Watch for changes in a directory or file at the specified path and triggers the appropriate events.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to watch.
    //$ arg1:   bool watch => Whether to start watching for changes (default is true).
    //$ arg2:   bool resursive => Whether to watch all files/folders recursively (default is false).
    //$ return: void_r => The result of the watch operation.
    void_r watch_for_changes(
        path_t objectPath,
        bool watch = true,
        bool resursive = false
    );

    //$ Watch for changes in a directory by its ID and triggers the appropriate events.
    //$ arg0:   directory::id_t dirID => The ID of the directory to watch.
    //$ arg1:   bool watch => Whether to start watching for changes (default is true).
    //$ arg2:   bool resursive => Whether to watch all files/folders recursively (default is false).
    //$ return: void_r => The result of the watch operation.
    void_r watch_for_changes(
        directory::id_t dirID,
        bool watch = true,
        bool resursive = false
    );

    //$ Watch for changes in a file by its ID and triggers the appropriate events.
    //$ arg0:   file::id_t fileID => The ID of the file to watch.
    //$ arg1:   bool watch => Whether to start watching for changes (default is true).
    //$ return: void_r => The result of the watch operation.
    void_r watch_for_changes(
        file::id_t fileID,
        bool watch = true
    );


    //$ Check if a directory or file at the specified path is being watched for changes.
    //$ arg0:   path_t objectPath => The absolute path of the directory or file to check.
    //$ return: bool_r => True if the directory or file is being watched, false otherwise.
    bool_r is_being_watched_for_changes(
        path_t objectPath
    );

    //$ Check if a directory by its ID is being watched for changes.
    //$ arg0:   directory::id_t dirID => The ID of the directory to check.
    //$ return: bool_r => True if the directory is being watched, false otherwise.
    bool_r is_being_watched_for_changes(
        directory::id_t dirID
    );

    //$ Check if a file by its ID is being watched for changes.
    //$ arg0:   file::id_t fileID => The ID of the file to check.
    //$ return: bool_r => True if the file is being watched, false otherwise.
    bool_r is_being_watched_for_changes(
        file::id_t fileID
    );


    //$ Clear the list of watched directories and files, stopping any active watches.
    //$ return: void_r => The result of the clear operation.
    void_r clear_watched_list();


    //$ Start polling for changes in the filesystem, triggering events for any changes detected.
    //$ return: void_r => The result of the start polling operation.
    void_r start_polling_for_changes();


    //$ Check if the filesystem is currently polling for changes.
    //$ return: bool_r => True if the filesystem is polling for changes, false otherwise.
    bool_r is_polling_for_changes();


    //$ Stop polling for changes in the filesystem.
    //$ return: void_r => The result of the stop polling operation.
    void_r stop_polling_for_changes();


    // ==============================================================================
    // Searching

    //$ Search for paths in the filesystem that match a given predicate.
    //$ arg0:   path_t rootPath => The absolute path to start the search from.
    //$ arg1:   search_predicate predicate => The predicate function to filter paths.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of paths that match the predicate.
    path_list_r search(
        path_t rootPath,
        search_predicate predicate,
        int recursionDepth,
        bool includeIgnored = false
    );

    //$ Search for paths in the filesystem that match a given predicate by directory ID.
    //$ arg0:   directory::id_t rootDirID => The ID of the directory to start the search from.
    //$ arg1:   search_predicate predicate => The predicate function to filter paths.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of paths that match the predicate.
    path_list_r search(
        directory::id_t rootDirID,
        search_predicate predicate,
        int recursionDepth,
        bool includeIgnored = false
    );


    // ==============================================================================
    // Searching by name or extension

    //$ Search for files and folders within the given directory by their names.
    //$ arg0:   path_t rootPath => The absolute path to start the search from.
    //$ arg1:   const eastl::vector<const wchar_t*>& names => The list of names to search for.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of paths that match the names.
    path_list_r search_by_name(
        path_t rootPath,
        const eastl::vector<const wchar_t*>& names,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        const search_predicate nameSearchPredicate = [&names](path_t path) {
            return eastl::any_of(names.begin(), names.end(), [&path](const wchar_t* name) {
                return path.filename().compare(name) == 0;
            });
        };
        return search(rootPath, nameSearchPredicate, recursionDepth, includeIgnored);
    }

    //$ Search for files and folders within the registered directory by their names.
    //$ arg0:   directory::id_t rootDirID => The ID of the directory to start the search from.
    //$ arg1:   const eastl::vector<const wchar_t*>& names => The list of names to search for.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of paths that match the names.
    path_list_r search_by_name(
        directory::id_t rootDirID,
        const eastl::vector<const wchar_t*>& names,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        const search_predicate nameSearchPredicate = [&names](path_t path) {
            return eastl::any_of(names.begin(), names.end(), [&path](const wchar_t* name) {
                return path.filename().compare(name) == 0;
            });
        };
        return search(rootDirID, nameSearchPredicate, recursionDepth, includeIgnored);
    }


    //$ Search for files within a registered directory by their extensions.
    //$ arg0:   path_t rootPath => The absolute path to start the search from.
    //$ arg1:   const eastl::vector<const wchar_t*>& extensions => The list of extensions to search for, dot excluded.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of paths that match the extensions.
    path_list_r search_by_extension(
        directory::id_t rootDirID,
        const eastl::vector<const wchar_t*>& extensions,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        const search_predicate extensionSearchPredicate = [&extensions](path_t path) {
            return eastl::any_of(extensions.begin(), extensions.end(), [&path](const wchar_t* ext) {
                return path.extension().compare(ext) == 0;
            });
        };
        return search(rootDirID, extensionSearchPredicate, recursionDepth, includeIgnored);
    }

    //$ Search for files within the given directory by their extensions.
    //$ arg0:   path_t rootPath => The absolute path to start the search from.
    //$ arg1:   const eastl::vector<const wchar_t*>& extensions => The list of extensions to search for, dot excluded.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of paths that match the extensions.
    path_list_r search_by_extension(
        path_t rootPath,
        const eastl::vector<const wchar_t*>& extensions,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        const search_predicate extensionSearchPredicate = [&extensions](path_t path) {
            return eastl::any_of(extensions.begin(), extensions.end(), [&path](const wchar_t* ext) {
                return path.extension().compare(ext) == 0;
            });
        };
        return search(rootPath, extensionSearchPredicate, recursionDepth, includeIgnored);
    }


    //$ Search for subdirectories only.
    //$ arg0:   path_t rootPath => The absolute path to start the search from.
    //$ arg1:   const eastl::vector<const wchar_t*>& names => The list of names to search for.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of subdirectories that match the names.
    path_list_r search_for_subdirectories(
        path_t rootPath,
        search_predicate predicate,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        const search_predicate directorySearchPredicate = [&predicate](path_t path) {
            return std::filesystem::is_directory(path) && predicate(path);
        };
        return search(rootPath, directorySearchPredicate, recursionDepth, includeIgnored);
    }

    //$ Search for subdirectories only.
    //$ arg0:   directory::id_t rootDirID => The ID of the directory to start the search from.
    //$ arg1:   search_predicate predicate => The predicate function to filter subdirectories.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of subdirectories that match the names.
    path_list_r search_for_subdirectories(
        directory::id_t rootDirID,
        search_predicate predicate,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        const search_predicate directorySearchPredicate = [&predicate](path_t path) {
            return std::filesystem::is_directory(path) && predicate(path);
        };
        return search(rootDirID, directorySearchPredicate, recursionDepth, includeIgnored);
    }


    //$ Search for files only.
    //$ arg0:   path_t rootPath => The absolute path to start the search from.
    //$ arg1:   search_predicate predicate => The predicate function to filter files.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of files that match the predicate.
    path_list_r search_for_files(
        path_t rootPath,
        search_predicate predicate,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        const search_predicate fileSearchPredicate = [&predicate](path_t path) {
            return std::filesystem::is_regular_file(path) && predicate(path);
        };
        return search(rootPath, fileSearchPredicate, recursionDepth, includeIgnored);
    }

    //$ Search for files only.
    //$ arg0:   directory::id_t rootDirID => The ID of the directory to start the search from.
    //$ arg1:   search_predicate predicate => The predicate function to filter files.
    //$ arg2:   int recursionDepth => The maximum depth of recursion for the search. Use -1 for complete recursion.
    //$ arg3:   bool includeIgnored => Whether to include ignored paths in the search (default is false).
    //$ return: path_list_r => The list of files that match the predicate.
    path_list_r search_for_files(
        directory::id_t rootDirID,
        search_predicate predicate,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        const search_predicate fileSearchPredicate = [&predicate](path_t path) {
            return std::filesystem::is_regular_file(path) && predicate(path);
        };
        return search(rootDirID, fileSearchPredicate, recursionDepth, includeIgnored);
    }


    // ==============================================================================
    // Listing contents (search helpers)

    //$ List all files and directories in the specified path.
    //$ arg0:   path_t rootPath => The absolute path to start listing from.
    //$ arg1:   int recursionDepth => The maximum depth of recursion for the listing. Use -1 for complete recursion.
    //$ arg2:   bool includeIgnored => Whether to include ignored paths in the listing (default is false).
    //$ return: path_list_r => The list of all files and directories in the specified path.
    path_list_r list_all(
        path_t rootPath,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        return search(rootPath, [](const path_t&) { return true; }, recursionDepth, includeIgnored);
    }

    //$ List all files and directories in the specified directory by ID.
    //$ arg0:   directory::id_t rootDirID => The ID of the directory to start listing from.
    //$ arg1:   int recursionDepth => The maximum depth of recursion for the listing. Use -1 for complete recursion.
    //$ arg2:   bool includeIgnored => Whether to include ignored paths in the listing (default is false).
    //$ return: path_list_r => The list of all files and directories in the specified directory.
    path_list_r list_all(
        directory::id_t rootDirID,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        return search(rootDirID, [](const path_t&) { return true; }, recursionDepth, includeIgnored);
    }


    //$ List all directories in the specified path.
    //$ arg0:   path_t rootPath => The absolute path to start listing directories from.
    //$ arg1:   int recursionDepth => The maximum depth of recursion for the listing. Use -1 for complete recursion.
    //$ arg2:   bool includeIgnored => Whether to include ignored paths in the listing (default is false).
    //$ return: path_list_r => The list of all directories in the specified path.
    path_list_r list_directories(
        path_t rootPath,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        return search_for_subdirectories(rootPath, [](const path_t&) { return true; }, recursionDepth, includeIgnored);
    }

    //$ List all directories in the specified directory by ID.
    //$ arg0:   directory::id_t rootDirID => The ID of the directory to start listing directories from.
    //$ arg1:   int recursionDepth => The maximum depth of recursion for the listing. Use -1 for complete recursion.
    //$ arg2:   bool includeIgnored => Whether to include ignored paths in the listing (default is false).
    //$ return: path_list_r => The list of all directories in the specified directory.
    path_list_r list_directories(
        directory::id_t rootDirID,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        return search_for_subdirectories(rootDirID, [](const path_t&) { return true; }, recursionDepth, includeIgnored);
    }


    //$ List all files in the specified path.
    //$ arg0:   path_t rootPath => The absolute path to start listing files from.
    //$ arg1:   int recursionDepth => The maximum depth of recursion for the listing. Use -1 for complete recursion.
    //$ arg2:   bool includeIgnored => Whether to include ignored paths in the listing (default is false).
    //$ return: path_list_r => The list of all files in the specified path.
    path_list_r list_files(
        path_t rootPath,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        return search_for_files(rootPath, [](const path_t&) { return true; }, recursionDepth, includeIgnored);
    }

    //$ List all files in the specified directory by ID.
    //$ arg0:   directory::id_t rootDirID => The ID of the directory to start listing files from.
    //$ arg1:   int recursionDepth => The maximum depth of recursion for the listing. Use -1 for complete recursion.
    //$ arg2:   bool includeIgnored => Whether to include ignored paths in the listing (default is false).
    //$ return: path_list_r => The list of all files in the specified directory.
    path_list_r list_files(
        directory::id_t rootDirID,
        int recursionDepth,
        bool includeIgnored = false
    ) {
        return search_for_files(rootDirID, [](const path_t&) { return true; }, recursionDepth, includeIgnored);
    }
}