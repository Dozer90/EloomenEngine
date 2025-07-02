#include "filesystem/filesystem.h"
#include "events/engine/events.h"

#include <EASTL/unordered_map.h>

#include <fstream>


using namespace eloo;
namespace stdfs = std::filesystem;

namespace {
    size_t gNextDirectoryID = 0;
    size_t gNextFileID = 0;
    eastl::unordered_map<filesystem::directory::id_t, filesystem::path_t> gDirectoryPaths;
    eastl::unordered_map<filesystem::file::id_t, filesystem::path_t> gFilePaths;

    bool get_directory_path(filesystem::directory::id_t dirID, filesystem::path_t& outPath) ELOO_NOEXCEPT {
        if (const auto found = gDirectoryPaths.find(dirID); found != gDirectoryPaths.end()) {
            outPath = found->second;
            return true;
        }
        return false;
    }

    bool get_file_path(filesystem::file::id_t fileID, filesystem::path_t& outPath) ELOO_NOEXCEPT {
        if (const auto found = gFilePaths.find(fileID); found != gFilePaths.end()) {
            outPath = found->second;
            return true;
        }
        return false;
    }
}

// ==============================================================================

filesystem::void_r filesystem::create(path_t path, bool overwriteFileIfExists) {
    std::error_code e;

    ELOO_RETURN_ERRC_IF(!resolve(path), errc::path_resolution_failed);

    const bool exists = stdfs::exists(path, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);

    if (!path.has_extension()) {

        // ------------------------------------------------
        // Creating a new directory

        ELOO_RETURN_ERRC_IF(exists && !stdfs::is_directory(path, e), errc::not_a_directory);
        stdfs::create_directories(path, e);
        ELOO_RETURN_ERRC_IF_FAILED(e);
        return {};
    }

    // ------------------------------------------------
    // Creating a new file

    if (exists) {
        ELOO_RETURN_ERRC_IF(stdfs::is_directory(path, e), errc::not_a_file);
        ELOO_RETURN_ERRC_IF(overwriteFileIfExists, errc::already_exists);
    }

    // Create the file
    stdfs::create_directories(path.parent_path(), e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    std::ofstream newFile(path);
    ELOO_RETURN_ERRC_IF(!newFile || !newFile.good(), errc::file_creation_failed);
    return {};
}

filesystem::directory_id_r filesystem::create_and_register_directory(path_t targetPath) {
    ELOO_RETURN_ERRC_IF_FUNC_FAILED(create(targetPath));
    return register_directory(targetPath);
}

filesystem::file_id_r filesystem::create_and_register_file(path_t targetPath, bool overwriteFileIfExists) {
    ELOO_RETURN_ERRC_IF_FUNC_FAILED(create(targetPath, overwriteFileIfExists));
    return register_file(targetPath);
}

// ------------------------------------------------------------------------------

filesystem::void_r filesystem::destroy(path_t targetPath) {
    std::error_code e;
    ELOO_RETURN_ERRC_IF(!resolve(targetPath), errc::path_resolution_failed);

    const bool exists = stdfs::exists(targetPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    ELOO_RETURN_ERRC_IF(exists, errc::not_found);

    const bool isDirectory = stdfs::is_directory(targetPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    if (isDirectory) {
        stdfs::remove_all(targetPath, e);
        ELOO_RETURN_ERRC_IF_FAILED(e);
    } else {
        stdfs::remove(targetPath, e);
        ELOO_RETURN_ERRC_IF_FAILED(e);
    }

    return {};
}

filesystem::void_r filesystem::destroy(directory::id_t targetID) {
    path_t targetPath;
    ELOO_RETURN_ERRC_IF(!get_directory_path(targetID, targetPath), errc::invalid_id);
    return destroy(targetPath);
}

filesystem::void_r filesystem::destroy(file::id_t targetID) {
    path_t targetPath;
    ELOO_RETURN_ERRC_IF(!get_file_path(targetID, targetPath), errc::invalid_id);
    return destroy(targetPath);
}

// ------------------------------------------------------------------------------

filesystem::void_r filesystem::clear_directory(path_t targetPath) {
    std::error_code e;
    ELOO_RETURN_ERRC_IF(!resolve(targetPath), errc::path_resolution_failed);

    const bool isDirectory = stdfs::is_directory(targetPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    ELOO_RETURN_ERRC_IF(!isDirectory, errc::not_a_directory);

    const bool exists = stdfs::exists(targetPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    ELOO_RETURN_ERRC_IF(exists, errc::invalid_path);

    stdfs::remove_all(targetPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    stdfs::create_directory(targetPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);

    return {};
}

filesystem::void_r filesystem::clear_directory(directory::id_t dirID) {
    path_t targetPath;
    ELOO_RETURN_ERRC_IF(!get_directory_path(dirID, targetPath), errc::invalid_id);
    return clear_directory(targetPath);
}

// ------------------------------------------------------------------------------

filesystem::void_r filesystem::rename(path_t objectPath, const path_t& newName) {
    std::error_code e;
    ELOO_RETURN_ERRC_IF(!resolve(objectPath), errc::path_resolution_failed);

    const bool exists = stdfs::exists(objectPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    ELOO_RETURN_ERRC_IF(!exists, errc::not_found);

    ELOO_RETURN_ERRC_IF(newName.empty() || newName.has_parent_path(), errc::invalid_name);

    stdfs::rename(objectPath, objectPath.parent_path() / newName, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    return {};
}

filesystem::void_r filesystem::rename(directory::id_t dirID, const path_t& newName) {
    path_t objectPath;
    ELOO_RETURN_ERRC_IF(!get_directory_path(dirID, objectPath), errc::invalid_id);
    return filesystem::rename(objectPath, newName);
}

filesystem::void_r filesystem::rename(file::id_t fileID, const path_t& newName) {
    path_t objectPath;
    ELOO_RETURN_ERRC_IF(!get_file_path(fileID, objectPath), errc::invalid_id);
    return filesystem::rename(objectPath, newName);
}

// ------------------------------------------------------------------------------

filesystem::void_r filesystem::copy(path_t objectPath, path_t targetPath, copy_opts opts) {
    ELOO_RETURN_ERRC_IF(!resolve(objectPath), errc::path_resolution_failed);
    ELOO_RETURN_ERRC_IF(!resolve(targetPath), errc::path_resolution_failed);

    std::error_code e;
    const bool exists = stdfs::exists(objectPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    ELOO_RETURN_ERRC_IF(!exists, errc::not_found);

    const bool isDirectory = stdfs::is_directory(objectPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);

    if (isDirectory) {
        stdfs::copy(objectPath, targetPath, opts, e);
        ELOO_RETURN_ERRC_IF_FAILED(e);
    } else {
        stdfs::copy_file(objectPath, targetPath, opts, e);
        ELOO_RETURN_ERRC_IF_FAILED(e);
    }

    return {};
}

filesystem::void_r filesystem::copy(directory::id_t dirID, path_t targetPath, copy_opts opts) {
    path_t objectPath;
    ELOO_RETURN_ERRC_IF(!get_directory_path(dirID, objectPath), errc::invalid_id);
    return filesystem::copy(objectPath, targetPath, opts);
}

filesystem::void_r filesystem::copy(file::id_t fileID, path_t targetPath, copy_opts opts) {
    path_t objectPath;
    ELOO_RETURN_ERRC_IF(!get_file_path(fileID, objectPath), errc::invalid_id);
    return filesystem::copy(objectPath, targetPath, opts);
}

// ------------------------------------------------------------------------------

filesystem::void_r filesystem::move(path_t objectPath, path_t destinationPath) {
    ELOO_RETURN_ERRC_IF(!resolve(objectPath), errc::path_resolution_failed);
    ELOO_RETURN_ERRC_IF(!resolve(destinationPath), errc::path_resolution_failed);

    std::error_code e;
    const bool exists = stdfs::exists(objectPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    ELOO_RETURN_ERRC_IF(!exists, errc::not_found);

    stdfs::rename(objectPath, destinationPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    return {};
}

filesystem::void_r filesystem::move(file::id_t fileID, path_t destinationPath) {
    path_t objectPath;
    ELOO_RETURN_ERRC_IF(!get_file_path(fileID, objectPath), errc::invalid_id);
    return filesystem::move(objectPath, destinationPath);
}

filesystem::void_r filesystem::move(directory::id_t dirID, path_t destinationPath) {
    path_t objectPath;
    ELOO_RETURN_ERRC_IF(!get_directory_path(dirID, objectPath), errc::invalid_id);
    return filesystem::move(objectPath, destinationPath);
}

// ------------------------------------------------------------------------------

filesystem::void_r filesystem::set_perms(path_t objectPath, perms permissions) {
    ELOO_RETURN_ERRC_IF(!resolve(objectPath), errc::path_resolution_failed);

    std::error_code e;
    const bool exists = stdfs::exists(objectPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    ELOO_RETURN_ERRC_IF(!exists, errc::not_found);

    stdfs::permissions(objectPath, permissions, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    return {};
}

filesystem::void_r filesystem::set_perms(directory::id_t dirID, perms permissions) {
    path_t objectPath;
    ELOO_RETURN_ERRC_IF(!get_directory_path(dirID, objectPath), errc::invalid_id);
    return filesystem::set_perms(objectPath, permissions);
}

filesystem::void_r filesystem::set_perms(file::id_t fileID, perms permissions) {
    path_t objectPath;
    ELOO_RETURN_ERRC_IF(!get_file_path(fileID, objectPath), errc::invalid_id);
    return filesystem::set_perms(objectPath, permissions);
}

// ==============================================================================

void filesystem::normalize_path_slashes(path_t& path) {
    // Normalize slashes to the platform's preferred separator
    path.make_preferred();
}

filesystem::path_r filesystem::canonicalize_path(path_t path) {
    ELOO_RETURN_ERRC_IF(!resolve(path), errc::path_resolution_failed);

    // Canonicalize the path to its absolute form, resolving any relative components
    std::error_code e;
    path = stdfs::canonical(path, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
}

bool filesystem::resolve(path_t& path) ELOO_NOEXCEPT {
    std::error_code e;

    if (path.empty()) {
        return false;
    }

    if (!path.is_absolute()) {
        const auto workingDir = get_working_directory();
        if (!workingDir || workingDir->empty()) {
            return false;
        }
        path = *workingDir / path;
    }
    return true;
}

bool filesystem::resolve(path_t& path, directory::id_t dirID) ELOO_NOEXCEPT {
    if (const auto found = gDirectoryPaths.find(dirID); found != gDirectoryPaths.end()) {
        path = found->second / path;
        return resolve(path);
    }
    return false;
}

// ==============================================================================

filesystem::path_r filesystem::get_working_directory() EA_NOEXCEPT {
    std::error_code e;
    const path_t wp = stdfs::current_path(e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    return wp;
}

filesystem::void_r filesystem::set_working_directory(path_t targetPath) EA_NOEXCEPT {
    ELOO_RETURN_ERRC_IF(!resolve(targetPath), errc::path_resolution_failed);

    std::error_code e;
    stdfs::current_path(targetPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);

    return {};
}

filesystem::void_r filesystem::set_working_directory(directory::id_t id) EA_NOEXCEPT {
    path_t targetPath;
    ELOO_RETURN_ERRC_IF(!get_directory_path(id, targetPath), errc::invalid_id);
    return set_working_directory(targetPath);
}

// ==============================================================================

filesystem::directory_id_r filesystem::register_directory(path_t targetPath) {
    std::error_code e;
    ELOO_RETURN_ERRC_IF(!resolve(targetPath), errc::path_resolution_failed);

    stdfs::create_directories(targetPath, e);
    ELOO_RETURN_ERRC_IF_FAILED(e);
    const auto dirID = directory::id_t(gDirectoryPaths.size() + 1);
    gDirectoryPaths[dirID] = targetPath;
    return dirID;
}