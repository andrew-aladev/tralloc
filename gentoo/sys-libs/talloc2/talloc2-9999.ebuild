# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: /var/cvsroot/gentoo-x86/net-im/vacuum/vacuum-9999.ebuild,v 1.10 2013/09/21 12:55:34 tomwij Exp $

EAPI="5"

inherit cmake-utils git-2

DESCRIPTION="Lightweight implementation of simplified talloc api."
HOMEPAGE="https://github.com/andrew-aladev/talloc2"
EGIT_REPO_URI="git://github.com/andrew-aladev/talloc2.git"

LICENSE="LGPL-3+"
SLOT="0/9999"
KEYWORDS=""

IUSE="debug +shared-libs static-libs test"

IUSE_FEATURES="destructor reference"
for feature in ${IUSE_FEATURES}; do
    IUSE="${IUSE} talloc2_features_${feature}"
done

IUSE_UTILS="buffer dynarr"
for util in ${IUSE_UTILS}; do
    IUSE="${IUSE} talloc2_utils_${util}"
done

RDEPEND=""
DEPEND="${RDEPEND}"

src_unpack() {
    git-2_src_unpack
}

src_configure() {
    local mycmakeargs=(
        $(cmake-utils_use debug       TALLOC_DEBUG)
        $(cmake-utils_use shared-libs TALLOC_SHARED)
        $(cmake-utils_use static-libs TALLOC_STATIC)
        $(cmake-utils_use test        TALLOC_TEST)
        
        $(cmake-utils_use talloc2_features_destructor TALLOC_DESTRUCTOR)
        $(cmake-utils_use talloc2_features_reference  TALLOC_REFERENCE)
        
        $(cmake-utils_use talloc2_utils_buffer TALLOC_UTILS_BUFFER)
        $(cmake-utils_use talloc2_utils_dynarr TALLOC_UTILS_DYNARR)
    )
    cmake-utils_src_configure
}

src_test() {
    cmake-utils_src_test
}

src_install() {
    cmake-utils_src_install
}
