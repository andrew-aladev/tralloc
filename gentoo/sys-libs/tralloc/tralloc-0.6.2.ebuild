# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2

EAPI="5"

inherit cmake-multilib cmake-utils

DESCRIPTION="Lightweight implementation of simplified talloc api."
HOMEPAGE="https://github.com/andrew-aladev/tralloc"
SRC_URI="https://github.com/andrew-aladev/tralloc/archive/v${PV}.tar.gz"

LICENSE="LGPL-3+"
SLOT="0/9999"
KEYWORDS=""

IUSE="debug shared-libs static-libs man test"

IUSE_FEATURES="destructor reference"
for feature in ${IUSE_FEATURES}; do
    IUSE="${IUSE} tralloc_features_${feature}"
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

src_configure() {
    local mycmakeargs=(
        $(cmake-utils_use debug       TRALLOC_DEBUG)
        $(cmake-utils_use shared-libs TRALLOC_SHARED)
        $(cmake-utils_use static-libs TRALLOC_STATIC)
        $(cmake-utils_use man         TRALLOC_MAN)
        $(cmake-utils_use test        TRALLOC_TEST)
        
        $(cmake-utils_use tralloc_features_destructor TRALLOC_DESTRUCTOR)
        $(cmake-utils_use tralloc_features_reference  TRALLOC_REFERENCE)
        
        $(cmake-utils_use tralloc_utils_buffer TRALLOC_UTILS_BUFFER)
    )
    cmake-multilib_src_configure
}

src_test() {
    cmake-multilib_src_test
}

src_install() {
    cmake-multilib_src_install
}
