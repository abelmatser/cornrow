#include "Model.h"

#include <QPen>

Model::Model(const std::vector<float>& freqTable,
             const std::vector<float>& qTable,
             QObject *parent) :
    QObject(parent),
    m_freqTable(freqTable),
    m_qTable(qTable)
{
}

Model::Model(QObject *parent) :
    QObject(parent),
    m_freqTable(common::twelfthOctaveBandsTable),
    m_qTable(common::qTable)
{
    connect(this, &Model::typeChanged, this, &Model::onCurrentFilterParameterChanged);
    connect(this, &Model::freqChanged, this, &Model::onCurrentFilterParameterChanged);
    connect(this, &Model::gainChanged, this, &Model::onCurrentFilterParameterChanged);
    connect(this, &Model::qChanged, this, &Model::onCurrentFilterParameterChanged);
}

void Model::addFilter()
{
    m_filters.append(Filter());
    emit filterAdded();
    emit filterCountChanged();

    setCurrentFilter(m_filters.size()-1);
}

void Model::deleteFilter()
{
    if (m_curIndex < 0) return;

    m_filters.removeAt(m_curIndex);
    emit filterRemoved(m_curIndex);
    emit filterCountChanged();

    setCurrentFilter(m_curIndex);
}

void Model::setCurrentFilter(int i)
{
    if (m_filters.empty()) {
        m_curFilter = nullptr;
        if (m_curIndex == -1) return;
        m_curIndex = -1;

        emit currentFilterChanged();
    } else {
        if (i >= m_filters.size()) {
            m_curIndex = m_filters.size()-1;
        } else {
            m_curIndex = i;
        }
        m_curFilter = &(m_filters[m_curIndex]);

        emit currentFilterChanged();
        emit typeChanged();
        emit freqChanged();
        emit freqSliderChanged();
        emit gainChanged();
        emit qChanged();
        emit qSliderChanged();
    }
}

int Model::filterCount() const
{
    return m_filters.size();
}

int Model::currentFilter() const
{
    return m_curIndex;
}

int Model::type() const
{
    return static_cast<int>(m_curFilter->t);
}

void Model::setType(int type)
{
    common::FilterType t = static_cast<common::FilterType>(type);
    if (m_curFilter->t == t) return;

    m_curFilter->t = t;
    emit typeChanged();
}

QString Model::freqReadout() const
{
    float value = m_curFilter ? m_freqTable.at(m_curFilter->f) : 1000.0;

    if (value < 1.0) return QString::number(value, 'f', 2);
    else if (value < 100.0) return QString::number(value, 'f', 1);
    else return QString::number(value, 'f', 0);
}

void Model::stepFreq(int i)
{
    int idx = m_curFilter->f + i;
    if (idx < 0) return;
    if (idx > m_freqTable.size()-1) return;
    if (m_curFilter->f == idx) return;

    m_curFilter->f = idx;

    emit freqChanged();
    emit freqSliderChanged();
}

float Model::freqSlider() const
{
    if (!m_curFilter) return (float)m_defaultFreq/(float)(m_freqTable.size()-1);

    return (float)m_curFilter->f/(float)(m_freqTable.size()-1);
}

void Model::setFreqSlider(float f)
{
    // @TODO(mawe): this can crash we removing last filter band
    int idx = qRound(f*(m_freqTable.size()-1));
    if (m_curFilter->f == idx) return;

    m_curFilter->f = idx;
    emit freqChanged();
}

float Model::gain() const
{
    if (!m_curFilter) return 0.0f;

    return m_curFilter->g;
}

void Model::setGain(float g)
{
    if (m_curFilter->g == g) return;
    if (g > m_maxGain) return;
    if (g < m_minGain) return;

    m_curFilter->g = g;
    emit gainChanged();
}

QString Model::qReadout() const
{
    float value = m_curFilter ? m_qTable.at(m_curFilter->q) : 0.70;

    if (value < 1.0) return QString::number(value, 'f', 2);
    else if (value < 10.0) return QString::number(value, 'f', 1);
    else return QString::number(value, 'f', 0);
}

void Model::stepQ(int i)
{
    int idx = m_curFilter->q + i;
    if (idx < 0) return;
    if (idx > m_qTable.size()-1) return;
    if (m_curFilter->q == idx) return;

    m_curFilter->q = idx;

    emit qChanged();
    emit qSliderChanged();
}

float Model::qSlider() const
{
    if (!m_curFilter) return (float)m_defaultQ/(float)(m_qTable.size()-1);

    return (float)m_curFilter->q/(float)(m_qTable.size()-1);
}

void Model::setQSlider(float q)
{
    int idx = qRound(q*(m_qTable.size()-1));
    if (m_curFilter->q == idx) return;

    m_curFilter->q = idx;
    emit qChanged();
}

void Model::onCurrentFilterParameterChanged()
{
    if (m_curFilter) {
        emit filterChanged(m_curIndex, static_cast<uchar>(m_curFilter->t), m_freqTable.at(m_curFilter->f), m_curFilter->g, m_qTable.at(m_curFilter->q));
    } else {
        emit filterChanged(m_curIndex, 0, 0.0f, 0.0f, 0.0f);
    }
}
