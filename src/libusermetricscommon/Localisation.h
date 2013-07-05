/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#ifndef USERMETRICSCOMMON_LOCALISATION_H_
#define USERMETRICSCOMMON_LOCALISATION_H_

#include <libintl.h>

static inline char * _(const char *__msgid) {
	return gettext(__msgid);
}

static inline char * _(const char *__domainname, const char *__msgid) {
	return dgettext(__domainname, __msgid);
}

static inline char * _(const char *__domainname, const char *__msgid,
		int __category) {
	return dcgettext(__domainname, __msgid, __category);
}

static inline char * _(const char *__msgid1, const char *__msgid2,
		unsigned long int __n) {
	return ngettext(__msgid1, __msgid2, __n);

}

static inline char * _(const char *__domainname, const char *__msgid1,
		const char *__msgid2, unsigned long int __n) {
	return dngettext(__domainname, __msgid1, __msgid2, __n);
}

static inline char * _(const char *__domainname, const char *__msgid1,
		const char *__msgid2, unsigned long int __n, int __category) {
	return dcngettext(__domainname, __msgid1, __msgid2, __n, __category);
}

#endif // USERMETRICSCOMMON_LOCALISATION_H_
