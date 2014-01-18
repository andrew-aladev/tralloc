# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2

EAPI="5"

inherit cmake-multilib cmake-utils git-2

DESCRIPTION="Lightweight implementation of simplified talloc api."
HOMEPAGE="https://github.com/andrew-aladev/tralloc"
EGIT_REPO_URI="git://github.com/andrew-aladev/tralloc.git"

LICENSE="LGPL-3+"
SLOT="0/9999"
KEYWORDS=""

IUSE="debug shared-libs static-libs man test"

IUSE_EXTENSIONS="destructor reference pool"
for extension in ${IUSE_EXTENSIONS}; do
    IUSE="${IUSE} tralloc_extensions_${extension}"
done

IUSE_UTILS="buffer"
for util in ${IUSE_UTILS}; do
    IUSE="${IUSE} tralloc_utils_${util}"
done

RDEPEND="
    virtual/libc
    man? ( app-text/asciidoc )
"
DEPEND="${RDEPEND}"

src_unpack() {
    git-2_src_unpack
}

src_configure() {
    local mycmakeargs=(
        $(cmake-utils_use debug       TRALLOC_DEBUG)
        $(cmake-utils_use shared-libs TRALLOC_SHARED)
        $(cmake-utils_use static-libs TRALLOC_STATIC)
        $(cmake-utils_use man         TRALLOC_MAN)
        $(cmake-utils_use test        TRALLOC_TEST)
        
        $(cmake-utils_use tralloc_extensions_destructor TRALLOC_DESTRUCTOR)
        $(cmake-utils_use tralloc_extensions_reference  TRALLOC_REFERENCE)
        $(cmake-utils_use tralloc_extensions_pool       TRALLOC_POOL)
        $(cmake-utils_use tralloc_utils_buffer          TRALLOC_UTILS_BUFFER)
    )
    cmake-multilib_src_configure
}

src_test() {
    cmake-multilib_src_test
}

src_install() {
    cmake-multilib_src_install
}
