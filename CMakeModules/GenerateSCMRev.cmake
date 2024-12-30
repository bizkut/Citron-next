# SPDX-FileCopyrightText: 2019 citron Emulator Project
# SPDX-License-Identifier: GPL-2.0-or-later

# Gets a UTC timestamp and sets the provided variable to it
function(get_timestamp _var)
    string(TIMESTAMP timestamp UTC)
    set(${_var} "${timestamp}" PARENT_SCOPE)
endfunction()

# generate git/build information
include(GetGitRevisionDescription)
if(NOT GIT_REF_SPEC)
    get_git_head_revision(GIT_REF_SPEC GIT_REV)
endif()
if(NOT GIT_DESC)
    git_describe(GIT_DESC --always --long --dirty)
endif()
if (NOT GIT_BRANCH)
  git_branch_name(GIT_BRANCH)
endif()
get_timestamp(BUILD_DATE)

# Generate cpp with Git revision from template
# Also if this is a CI build, add the build name (ie: Nightly, Canary) to the scm_rev file as well
set(REPO_NAME "")
set(BUILD_VERSION "1.0.0")
set(BUILD_ID ${DISPLAY_VERSION})
set(BUILD_FULLNAME "Beta ${BUILD_VERSION} ")

configure_file(scm_rev.cpp.in scm_rev.cpp @ONLY)
