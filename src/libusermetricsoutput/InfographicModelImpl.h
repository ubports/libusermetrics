
#ifndef USERMETRICSOUTPUT_INFOGRAPHICMODELIMPL
#define USERMETRICSOUTPUT_INFOGRAPHICMODELIMPL

#include <libusermetricsoutput/InfographicModel.h>
#include <libusermetricsoutput/InfographicData.h>

#include <QtCore/QScopedPointer>

namespace UserMetricsOutput {

class InfographicModelImpl: public InfographicModel {

public:
	typedef QSharedPointer<InfographicData> InfographicDataPtr;
	typedef QMultiMap<QString, InfographicDataPtr> FakeDataMap;

	InfographicModelImpl(QObject *parent = 0);

	virtual ~InfographicModelImpl();

	virtual QString label() const;

	virtual QString username() const;

	virtual void setUsername(const QString &username);

	virtual InfographicColorTheme * firstColor() const;

	virtual QAbstractItemModel *firstMonth() const;

	virtual int currentDay() const;

	virtual InfographicColorTheme * secondColor() const;

	virtual QAbstractItemModel *secondMonth() const;

protected Q_SLOTS:
	virtual void nextDataSourceSlot();

	virtual void readyForDataChangeSlot();

protected:
	void nextFakeData();

	void generateFakeData();

	void loadFakeData();

	void finishSetFakeData();

	QString m_label;
	QScopedPointer<InfographicColorThemeImpl> m_firstColor;
	QScopedPointer<QVariantListModel> m_firstMonth;
	QScopedPointer<InfographicColorThemeImpl> m_secondColor;
	QScopedPointer<QVariantListModel> m_secondMonth;
	int m_currentDay;
	QString m_username;
	FakeDataMap::const_iterator m_dataIndex;
	InfographicDataPtr m_newData;
	FakeDataMap m_fakeData;



//	Q_DECLARE_PRIVATE(InfographicModelImpl)
};

}

#endif /* USERMETRICSOUTPUT_INFOGRAPHICMODELIMPL */
