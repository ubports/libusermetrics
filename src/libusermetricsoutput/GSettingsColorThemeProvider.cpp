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

#include <libusermetricscommon/Localisation.h>
#include <libusermetricsoutput/GSettingsColorThemeProvider.h>
#include <libusermetricsoutput/ColorThemeImpl.h>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <QtXmlPatterns/QXmlSchemaValidator>

using namespace std;
using namespace UserMetricsOutput;

static const QString COLOR_BASEDIR("/usr/share/libusermetrics/themes/");

GSettingsColorThemeProvider::GSettingsColorThemeProvider(QObject *parent) :
		ColorThemeProvider(parent) {
	if (qEnvironmentVariableIsSet("XDG_DATA_DIRS")) {
		m_baseDirs = QString::fromUtf8(qgetenv("XDG_DATA_DIRS")).split(':');
	}

	if (m_baseDirs.isEmpty()) {
		m_baseDirs << COLOR_BASEDIR;
	}

	QString schemaBaseDir(COLOR_BASEDIR);
	if (qEnvironmentVariableIsSet("USERMETRICS_COLOR_SCHEMA_BASEDIR")) {
		schemaBaseDir = QString::fromUtf8(
				qgetenv("USERMETRICS_COLOR_SCHEMA_BASEDIR"));
	}

	m_schema.load(
			QUrl::fromLocalFile(
					QDir(schemaBaseDir).filePath("color-theme.xsd")));

	if (m_schema.isValid()) {
		if (qEnvironmentVariableIsSet("USERMETRICS_NO_COLOR_SETTINGS")) {
			loadXmlColors("default");
		} else {
			this->m_settings.reset(
					new QGSettings("com.canonical.UserMetrics",
							"/com/canonical/UserMetrics/"));
			loadXmlColors(m_settings->get("theme").toString());
			connect(m_settings.data(), SIGNAL(changed(const QString &)), this,
			SLOT(changed(const QString &)));
		}

	} else {
		loadBlankColors();
	}
}

GSettingsColorThemeProvider::~GSettingsColorThemeProvider() {
}

void GSettingsColorThemeProvider::loadBlankColors() {
	ColorThemePtr blankTheme(
			ColorThemePtr(new ColorThemeImpl(QColor(), QColor(), QColor())));
	m_colorThemes << ColorThemePtrPair(blankTheme, blankTheme);
	m_color = m_colorThemes.begin();
}

QString GSettingsColorThemeProvider::convertPath(const QString &base,
		const QString& theme) {
	return QDir(base).filePath(QString(theme).append(".xml"));
}

void GSettingsColorThemeProvider::loadXmlColors(const QString &theme) {
	QXmlSchemaValidator validator(m_schema);

	QFile file;

	for (const QString &baseDir : m_baseDirs) {
		file.setFileName(convertPath(baseDir, theme));
		if (file.exists()) {
			break;
		}
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning()
				<< QString(_("Cannot open XML file '%1' for reading")).arg(
						file.fileName());
		loadBlankColors();
		return;
	}

	if (!validator.validate(&file, QUrl::fromLocalFile(file.fileName()))) {
		loadBlankColors();
		return;
	}

	file.reset();
	QXmlStreamReader xml(&file);

	while (!xml.atEnd() && !xml.hasError()) {
		QXmlStreamReader::TokenType token = xml.readNext();

		/* If token is just StartDocument, we'll go to next.*/
		if (token == QXmlStreamReader::StartDocument) {
			continue;
		}

		if (token == QXmlStreamReader::StartElement) {
			/* If it's named themes, we'll go to the next.*/
			if (xml.name() == "themes") {
				continue;
			}

			if (xml.name() == "theme") {
				parseTheme(xml);
			}
		}
	}

	m_color = m_colorThemes.begin();
}

void GSettingsColorThemeProvider::parseTheme(QXmlStreamReader & xml) {
	/* Let's check that we're really getting a theme. */
	if (xml.tokenType() != QXmlStreamReader::StartElement
			&& xml.name() == "theme") {
		return;
	}

	ColorThemePtr foregroundTheme;
	ColorThemePtr backgroundTheme;

	xml.readNext();

	/*
	 * We're going to loop over the things because the order might change.
	 * We'll continue the loop until we hit an EndElement named theme.
	 */
	while (!(xml.tokenType() == QXmlStreamReader::EndElement
			&& xml.name() == "theme")) {
		if (xml.tokenType() == QXmlStreamReader::StartElement) {
			QXmlStreamAttributes attributes = xml.attributes();

			if (attributes.hasAttribute("start")
					&& attributes.hasAttribute("main")
					&& attributes.hasAttribute("end")) {

				ColorThemePtr theme(
						new ColorThemeImpl(
								QColor(attributes.value("start").toString()),
								QColor(attributes.value("main").toString()),
								QColor(attributes.value("end").toString())));

				if (xml.name() == "foreground") {
					foregroundTheme = theme;
				} else if (xml.name() == "background") {
					backgroundTheme = theme;
				}
			}

		}
		xml.readNext();
	}

	if (!foregroundTheme.isNull() && !backgroundTheme.isNull()) {
		m_colorThemes << ColorThemePtrPair(foregroundTheme, backgroundTheme);
	}
}

ColorThemePtrPair GSettingsColorThemeProvider::getColorTheme(
		const QString &dataSetId) {
	ColorThemePtrPair result;

	map_const_iterator it(m_colorThemeMap.constFind(dataSetId));
	if (it == m_colorThemeMap.constEnd()) {
		// get the next available color
		result = *m_color;

		++m_color;
		if (m_color == m_colorThemes.end()) {
			m_color = m_colorThemes.begin();
		}

		m_colorThemeMap.insert(dataSetId, result);
	} else {
		// there was a mapped value
		result = *it;
	}

	return result;
}

void GSettingsColorThemeProvider::changed(const QString &key) {
	if (key != "theme") {
		return;
	}
	QString theme(m_settings->get(key).toString());
	m_colorThemes.clear();
	m_colorThemeMap.clear();
	loadXmlColors(theme);
}
