#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "processviewer.h"

#include "helpers/colorhelper.h"

#include "models/station.h"
#include "models/project.h"

#include "colordelegate.h"

extern SpDataProviderInterface g_dataProvider;

ProcessViewer::ProcessViewer(QWidget *parent)
    : Viewer(parent)
{
    _vbox10Widget = /*new QWidget*/(this);
    _vbox10 = new QVBoxLayout(_vbox10Widget);
    _vbox10->setContentsMargins(0, 0, 0, 0);
    _vbox10->setSpacing(0);

    // left
    _hbox11Widget = new QWidget(this);
    _hbox11 = new QHBoxLayout(_hbox11Widget);
    //    _hbox11->setContentsMargins(0, 0, 0, 0);

    //    _label1 = new QLabel(_hbox11Widget);
    //    _label1->setText(tr("视图类别"));

    //    _cb1 = new QComboBox(_hbox11Widget);

    _label_selectYear = new QLabel(tr("年"), _hbox11Widget);
    _sb_selectYear = new QSpinBox(_hbox11Widget);
    _sb_selectYear->setMinimum(1753);
    _sb_selectYear->setMaximum(7998);
    _sb_selectYear->setValue(QDate().currentDate().year());
    _pb_selectYear = new QPushButton(tr("选择年"), _hbox11Widget);


    _label_selectMonth = new QLabel(tr("月"), _hbox11Widget);
    _sb_selectMonth = new QSpinBox(_hbox11Widget);
    _sb_selectMonth->setMinimum(1);
    _sb_selectMonth->setMaximum(12);
    _sb_selectMonth->setValue(QDate().currentDate().month());
    _pb_selectMonth = new QPushButton(tr("选择月"), _hbox11Widget);
    //    _pb_selectShangxun = new QPushButton(tr("上旬"), _hbox11Widget);
    //    _pb_selectZhongxun = new QPushButton(tr("中旬"), _hbox11Widget);
    //    _pb_selectXiaxun = new QPushButton(tr("下旬"), _hbox11Widget);

    _label_startDateTime = new QLabel(tr("起始时间"), _hbox11Widget);
    _dte_startDateTime = new QDateTimeEdit(_hbox11Widget);
    _dte_startDateTime->setDisplayFormat(DEFAULT_DATETIME_FORMAT);
    _label_endDateTime = new QLabel(tr("结束时间"), _hbox11Widget);
    _dte_endDateTime = new QDateTimeEdit(_hbox11Widget);
    _dte_endDateTime->setDisplayFormat(DEFAULT_DATETIME_FORMAT);

    _viewOption = new QPushButton(tr("快速选择"), _hbox11Widget);
    auto menuScope = new QMenu(this);
    auto actionAll = menuScope->addAction(tr("全"));
    connect(actionAll, &QAction::triggered, [=]{
        _dte_startDateTime->setDateTime(_xAxis1->startDateTime());
        _dte_endDateTime->setDateTime(_xAxis1->endDateTime());
    });
    auto actionYear = menuScope->addAction(tr("年"));
    connect(actionYear, &QAction::triggered, [=]{
        auto currentStartDT = _xAxis1->startDateTime();
        _dte_startDateTime->setDateTime(
                    QDateTime(
                        QDate(currentStartDT.date().year(), 1, 1)));
        _dte_endDateTime->setDateTime(_xAxis1->startDateTime().addYears(1));
    });
    auto actionMonth = menuScope->addAction(tr("月"));
    connect(actionMonth, &QAction::triggered, [=]{
        auto currentStartDT = _xAxis1->startDateTime();
        _dte_startDateTime->setDateTime(
                    QDateTime(
                        QDate(currentStartDT.date().year(),
                              currentStartDT.date().month(), 1)));
        _dte_endDateTime->setDateTime(_xAxis1->startDateTime().addMonths(1));
    });
    auto actionShangxun = menuScope->addAction(tr("上旬"));
    connect(actionShangxun, &QAction::triggered, [=]{
        auto currentStartDT = _xAxis1->startDateTime();
        _dte_startDateTime->setDateTime(
                    QDateTime(
                        QDate(currentStartDT.date().year(),
                              currentStartDT.date().month(), 1)));
        _dte_endDateTime->setDateTime(_xAxis1->startDateTime().addDays(10));
    });
    auto actionZhongxun = menuScope->addAction(tr("中旬"));
    connect(actionZhongxun, &QAction::triggered, [=]{
        auto currentStartDT = _xAxis1->startDateTime();
        _dte_startDateTime->setDateTime(
                    QDateTime(
                        QDate(currentStartDT.date().year(),
                              currentStartDT.date().month(), 11)));
        _dte_endDateTime->setDateTime(_xAxis1->startDateTime().addDays(10));
    });
    auto actionXiaxun = menuScope->addAction(tr("下旬"));
    connect(actionXiaxun, &QAction::triggered, [=]{
        auto currentStartDT = _xAxis1->startDateTime();
        _dte_startDateTime->setDateTime(
                    QDateTime(
                        QDate(currentStartDT.date().year(),
                              currentStartDT.date().month(), 21)));
        _dte_endDateTime->setDateTime(
                    QDateTime(
                        QDate(currentStartDT.date().year(),
                              currentStartDT.date().month(), 1))
                    .addMonths(1));
    });
    //        auto actionHongyao = menuScope->addAction(tr("洪要"));
    _viewOption->setMenu(menuScope);
    _insertTo = new QPushButton(tr("插到"), _hbox11Widget);
    _insertTo->setMenu(new QMenu(this));
    _insertTo->menu()->addAction(tr("选择打开的图表"));
    connect(_insertTo, &QPushButton::pressed, [=](){
        //    auto *menu = _insertTo->menu();
        //    menu->addAction(tr("测试"));
        //    if (menu != nullptr)
        //    {
        //        for (auto action: menu->actions())
        //        {
        //            menu->removeAction(action);
        //        }
//        auto *mainWindow = qobject_cast<MainWindow *>(this->parent());
//        if (mainWindow != nullptr)
//        {
//            auto menu = new QMenu(this);
//            auto list = mainWindow->getChartViewWidgets();
//            for (auto widget: list)
//            {
//                menu->addAction(widget->chartName());
//            }
//            _insertTo->setMenu(menu);
//        }
        //    }

    });

    _generate = new QPushButton(tr("生成"), _hbox11Widget);
    auto generateMenu = new QMenu(this);
    auto actionSDProcess = generateMenu->addAction(tr("水流沙过程"));
    connect(actionSDProcess, &QAction::triggered, [=]() {
        QDateTime startdt, enddt;
        std::list<QString> stcds;

        QString path = QFileDialog::getExistingDirectory(this, tr("保存目录"));
        if (QDir(path).exists())
        {
            for (auto chart: _processCharts)
            {
                startdt = chart->startDateTime();
                enddt = chart->endDateTime();
                stcds.merge(chart->stcds().toStdList());
            }
            int secs = (enddt.toMSecsSinceEpoch() -
                        startdt.toMSecsSinceEpoch())/1000;
            int allsecs = stcds.size()*4*secs;
            QProgressDialog progress(tr("正在导出水流沙数据"),
                                     tr("取消"), 0, allsecs, this);
            progress.setWindowModality(Qt::WindowModal);

            int i = 0;
            for (const auto &sid: stcds)
            {
                if (progress.wasCanceled())
                {
                    return;
                }
                int base = i++*4*secs;
                progress.setValue(base);
                QString fileName = sid + "_" + startdt.toString("yyyyMMdd")
                        + "_" + enddt.toString("yyyyMMdd") + ".txt";
                QFile file(QDir(path).absoluteFilePath(fileName));
                file.open(QIODevice::WriteOnly);
                QTextStream out(&file);
                out.setFieldWidth(24);
                out.setFieldAlignment(QTextStream::AlignRight);

                ProcessSeries series;

                if (g_dataProvider) {
                    series.setDataProvider(g_dataProvider);
                }

                series.setSTCD(sid);
                series.setStartDateTime(startdt);
                series.setEndDateTime(enddt);

                series.genStageMap();
                series.genDischargeMap();
                series.genSedimentMap();

                auto stageMap = series.mStages().data().toStdMap();
                auto dischargeMap = series.mDischarges().data().toStdMap();
                auto sedimentMap = series.mSediments().data().toStdMap();

                std::map<QDateTime, DGSDSProecssNode> map;

                for (const auto &kv: stageMap)
                {
                    if (progress.wasCanceled())
                    {
                        file.close();
                        return;
                    }
                    progress.setValue(base +
                                      (kv.first.toMSecsSinceEpoch() -
                                       startdt.toMSecsSinceEpoch())/1000);
                    auto key = kv.first;
                    if (map.find(key) != map.end())
                    {
                        map[key].stage = kv.second;
                    }
                    else
                    {
                        DGSDSProecssNode node;
                        node.stage = kv.second;
                        map.insert({key, node});
                    }
                }
                for (const auto &kv: dischargeMap)
                {
                    if (progress.wasCanceled())
                    {
                        file.close();
                        return;
                    }
                    progress.setValue(base + secs +
                                      (kv.first.toMSecsSinceEpoch() -
                                       startdt.toMSecsSinceEpoch())/1000);
                    auto key = kv.first;
                    if (map.find(key) != map.end())
                    {
                        map[key].discharge = kv.second;
                    }
                    else
                    {
                        DGSDSProecssNode node;
                        node.discharge = kv.second;
                        map.insert({key, node});
                    }
                }
                for (const auto &kv: sedimentMap)
                {
                    if (progress.wasCanceled())
                    {
                        file.close();
                        return;
                    }
                    progress.setValue(base + 2*secs +
                                      (kv.first.toMSecsSinceEpoch() -
                                       startdt.toMSecsSinceEpoch())/1000);
                    auto key = kv.first;
                    if (map.find(key) != map.end())
                    {
                        map[key].sediment = kv.second;
                    }
                    else
                    {
                        DGSDSProecssNode node;
                        node.sediment = kv.second;
                        map.insert({key, node});
                    }
                }

                for (const auto &kv: map)
                {
                    if (progress.wasCanceled())
                    {
                        file.close();
                        return;
                    }
                    progress.setValue(base + 3*secs +
                                      (kv.first.toMSecsSinceEpoch() -
                                       startdt.toMSecsSinceEpoch())/1000);
                    out << kv.first.toString("yyyy-MM-dd hh:mm:ss") // length: 19
                        << kv.second.stage
                        << kv.second.discharge
                        << kv.second.sediment
                        << "\r\n";
                }
                file.close();
            }

            progress.setValue(allsecs);
            QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        }

    });
    _generate->setMenu(generateMenu);

    _hbox11->addWidget(_label_selectYear);
    _hbox11->addWidget(_sb_selectYear);
    _hbox11->addWidget(_pb_selectYear);

    _hbox11->addWidget(_label_selectMonth);
    _hbox11->addWidget(_sb_selectMonth);
    _hbox11->addWidget(_pb_selectMonth);
    //        _hbox11->addWidget(_pb_selectShangxun);
    //        _hbox11->addWidget(_pb_selectZhongxun);
    //        _hbox11->addWidget(_pb_selectXiaxun);

    _hbox11->addWidget(_label_startDateTime);
    _hbox11->addWidget(_dte_startDateTime);
    _hbox11->addWidget(_label_endDateTime);
    _hbox11->addWidget(_dte_endDateTime);

    _hbox11->addWidget(_viewOption);
    _hbox11->addWidget(_insertTo);
    _hbox11->addWidget(_generate);


    _spacer11 = new QSpacerItem(0, 0,
                                QSizePolicy::Expanding, QSizePolicy::Minimum);
    _hbox11->addItem(_spacer11);

    //        _hbox11->addWidget(_label1);
    //        _hbox11->addWidget(_cb1);

    _vbox10->addWidget(_hbox11Widget);
    _gridWidget = new QWidget(this);
    _grid = new QGridLayout(_gridWidget);
    _grid->setContentsMargins(0, 0, 0, 0);
    _grid->setSpacing(0);

    _buttonPlus1 = new QPushButton("+", _gridWidget);
    _buttonPlus1->setFont(QFont("Arial", 20));

    auto menuAddChart = new QMenu();
    auto stageAction = menuAddChart->addAction(tr("水位过程线"));
    connect(stageAction, &QAction::triggered, [=]() {
        addViewToSplitter(_splitter11, new StageChart);
    });
    auto dischargeAction = menuAddChart->addAction(tr("流量过程线"));
    connect(dischargeAction, &QAction::triggered, [=]() {
        addViewToSplitter(_splitter11, new DischargeChart);
    });
    auto sedimentAction = menuAddChart->addAction(tr("沙量过程线"));
    connect(sedimentAction, &QAction::triggered, [=] {
        addViewToSplitter(_splitter11, new SedimentChart);
    });

    _buttonPlus1->setMenu(menuAddChart);

    _buttonPlus1->setMinimumWidth(250);
    _buttonPlus1->setMinimumHeight(40);
    _buttonPlus1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    _xAxis1 = new ProcessXAxisWidget(_gridWidget);
    _xAxis1->setMinimumHeight(40);
    _xAxis1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    _grid->addWidget(_buttonPlus1, 0, 0);
    _grid->addWidget(_buttonPlus1, 0, 1);
    _grid->addWidget(_xAxis1, 0, 2);

    _vbox10->addWidget(_gridWidget);

    _splitter11 = new QSplitter(Qt::Vertical, this);
    _splitter11->setHandleWidth(4);

    this->addViewToSplitter(_splitter11, new StageChart);
    this->addViewToSplitter(_splitter11, new DischargeChart);
    this->addViewToSplitter(_splitter11, new SedimentChart);

    _vbox10->addWidget(_splitter11);

    connect(_pb_selectYear, &QPushButton::clicked, [=]() {
        int year = _sb_selectYear->value();
        QDate minDate(year, 1, 1);
        QDate maxDate(year + 1, 1, 1);
        _dte_startDateTime->setDateTime(QDateTime(minDate, QTime(0, 0)));
        _dte_endDateTime->setDateTime(QDateTime(maxDate, QTime(0, 0)));
    });

    connect(_pb_selectMonth, &QPushButton::clicked, [=]{
        int year = _sb_selectYear->value();
        int month = _sb_selectMonth->value();
        QDate minDate, maxDate;
        minDate.setDate(year, month, 1);
        if (month != 12)
        {
            maxDate.setDate(year, month + 1, 1);
        }
        else
        {
            maxDate.setDate(year + 1, 1, 1);
        }
        _dte_startDateTime->setDateTime(QDateTime(minDate, QTime(0, 0)));
        _dte_endDateTime->setDateTime(QDateTime(maxDate, QTime(0, 0)));
    });

    //        connect(_pb_selectShangxun, &QPushButton::clicked, [=]{
    //            int year = _sb_selectYear->value();
    //            int month = _sb_selectMonth->value();
    //            QDate minDate, maxDate;
    //            minDate.setDate(year, month, 1);
    //            maxDate.setDate(year, month, 11);
    //            _dte_startDateTime->setDateTime(QDateTime(minDate, QTime(0, 0)));
    //            _dte_endDateTime->setDateTime(QDateTime(maxDate, QTime(0, 0)));
    //        });

    //        connect(_pb_selectZhongxun, &QPushButton::clicked, [=]{
    //            int year = _sb_selectYear->value();
    //            int month = _sb_selectMonth->value();
    //            QDate minDate, maxDate;
    //            minDate.setDate(year, month, 11);
    //            maxDate.setDate(year, month, 21);
    //            _dte_startDateTime->setDateTime(QDateTime(minDate, QTime(0, 0)));
    //            _dte_endDateTime->setDateTime(QDateTime(maxDate, QTime(0, 0)));
    //        });

    //        connect(_pb_selectXiaxun, &QPushButton::clicked, [=]{
    //            int year = _sb_selectYear->value();
    //            int month = _sb_selectMonth->value();
    //            QDate minDate, maxDate;
    //            minDate.setDate(year, month, 21);
    //            if (month != 12)
    //            {
    //                maxDate.setDate(year, month + 1, 1);
    //            }
    //            else
    //            {
    //                maxDate.setDate(year + 1, 1, 1);
    //            }
    //            _dte_startDateTime->setDateTime(QDateTime(minDate, QTime(0, 0)));
    //            _dte_endDateTime->setDateTime(QDateTime(maxDate, QTime(0, 0)));
    //        });

    connect(_dte_startDateTime, &QDateTimeEdit::dateTimeChanged,
            [=](const QDateTime &dateTime) {
        _xAxis1->setStartDateTime(dateTime);
        for (auto chart: _processCharts)
        {
            chart->setStartDateTime(dateTime);
        }
        //            selectData(_dte_startDateTime->dateTime(), _dte_endDateTime->dateTime());
    });
    connect(_dte_endDateTime, &QDateTimeEdit::dateTimeChanged,
            [=](const QDateTime &dateTime) {
        _xAxis1->setEndDateTime(dateTime);
        for (auto chart: _processCharts)
        {
            chart->setEndDateTime(dateTime);
        }
        //            selectData(_dte_startDateTime->dateTime(), _dte_endDateTime->dateTime());
    });

    connect(_xAxis1, &ProcessXAxisWidget::viewChanged,
            [=](const QDateTime &start, const QDateTime &end) {
        for (auto chart: _processCharts)
        {
            chart->setStartDateTime(start);
            chart->setEndDateTime(end);
        }
    });
    this->setLayout(_vbox10);

    _pb_selectMonth->click();
}


void ProcessViewer::loadToolBar()
{
//    auto selectButton = new QPushButton(tr("选择时段"), _toolBar);
//    auto selectMenu = new QMenu(selectButton);
//    selectMenu->addAction(tr("选择年"));
//    selectButton->setMenu(selectMenu);
//    auto selectAction = _toolBar->addWidget(selectButton);
//    _toolBar->addSeparator();
//    auto start = _toolBar->addAction(tr("起始时间"));
//    _toolBar->addAction(tr("结束时间"));
//    _toolBar->addSeparator();


    //    _hbox11->setContentsMargins(0, 0, 0, 0);

    //    _label1 = new QLabel(_hbox11Widget);
    //    _label1->setText(tr("视图类别"));

    //    _cb1 = new QComboBox(_hbox11Widget);

    auto label_selectYear = new QLabel(tr("年"), _toolBar);
    _toolBar->addWidget(label_selectYear);

    auto sb_selectYear = new QSpinBox(_toolBar);
    sb_selectYear->setMinimum(1753);
    sb_selectYear->setMaximum(7998);
    sb_selectYear->setValue(QDate().currentDate().year());
    _toolBar->addWidget(sb_selectYear);

    auto pb_selectYear = new QPushButton(tr("选择年"), _toolBar);
    _toolBar->addWidget(pb_selectYear);

    _toolBar->addSeparator();

//    _label_selectMonth = new QLabel(tr("月"), _hbox11Widget);
//    _sb_selectMonth = new QSpinBox(_hbox11Widget);
//    _sb_selectMonth->setMinimum(1);
//    _sb_selectMonth->setMaximum(12);
//    _pb_selectMonth = new QPushButton(tr("选择月"), _hbox11Widget);
//    //        _pb_selectShangxun = new QPushButton(tr("上旬"), _hbox11Widget);
//    //        _pb_selectZhongxun = new QPushButton(tr("中旬"), _hbox11Widget);
//    //        _pb_selectXiaxun = new QPushButton(tr("下旬"), _hbox11Widget);

//    _label_startDateTime = new QLabel(tr("起始时间"), _hbox11Widget);
//    _dte_startDateTime = new QDateTimeEdit(_hbox11Widget);
//    _dte_startDateTime->setDisplayFormat(DEFAULT_DATETIME_FORMAT);
//    _label_endDateTime = new QLabel(tr("结束时间"), _hbox11Widget);
//    _dte_endDateTime = new QDateTimeEdit(_hbox11Widget);
//    _dte_endDateTime->setDisplayFormat(DEFAULT_DATETIME_FORMAT);

//    _viewOption = new QPushButton(tr("视图范围"), _hbox11Widget);
//    auto menuScope = new QMenu(this);
//    auto actionAll = menuScope->addAction(tr("全"));
//    auto actionYear = menuScope->addAction(tr("年"));
//    auto actionMonth = menuScope->addAction(tr("月"));
//    auto actionShangxun = menuScope->addAction(tr("上旬"));
//    auto actionZhongxun = menuScope->addAction(tr("中旬"));
//    auto actionXiaxun = menuScope->addAction(tr("下旬"));
//    auto actionHongyao = menuScope->addAction(tr("洪要"));
//    _viewOption->setMenu(menuScope);
//    _insertTo = new QPushButton(tr("插到"), _hbox11Widget);
//    _insertTo->setMenu(new QMenu(this));
//    _insertTo->menu()->addAction(tr("asdf"));
//    connect(_insertTo, &QPushButton::pressed, [=](){
////            auto *menu = _insertTo->menu();
////            menu->addAction(tr("测试"));
////            if (menu != nullptr)
////            {
////                for (auto *action: menu->actions())
////                {
////                    menu->removeAction(action);
////                }
//            auto *mainWindow = qobject_cast<MainWindow *>(this->parent());
//            if (mainWindow != nullptr)
//            {
//                auto menu = new QMenu(this);
//                auto list = mainWindow->getChartViewWidgets();
//                for (auto *widget: list)
//                {
//                    menu->addAction(widget->chartName());
//                }
//                _insertTo->setMenu(menu);
//            }
////            }

//    });

//    _generate = new QPushButton(tr("生成"), _hbox11Widget);
//    auto generateMenu = new QMenu(this);
//    auto actionSDProcess = generateMenu->addAction(tr("水流沙过程"));
//    connect(actionSDProcess, &QAction::triggered, [=]() {
//        QDateTime startdt, enddt;
//        std::list<QString> stcds;

//        QString path = QFileDialog::getExistingDirectory(this, tr("保存目录"));
//        if (QDir(path).exists())
//        {
//            for (auto *chart: _processCharts)
//            {
//                startdt = chart->startDateTime();
//                enddt = chart->endDateTime();
//                stcds.merge(chart->stcds().toStdList());
//            }
//            int secs = (enddt.toMSecsSinceEpoch() - startdt.toMSecsSinceEpoch())/1000;
//            int allsecs = stcds.size()*4*secs;
//            QProgressDialog progress(tr("正在导出水流沙数据"), tr("取消"), 0, allsecs, this);
//            progress.setWindowModality(Qt::WindowModal);

//            int i = 0;
//            for (const auto &sid: stcds)
//            {
//                if (progress.wasCanceled())
//                {
//                    return;
//                }
//                int base = i++*4*secs;
//                progress.setValue(base);
//                QString fileName = sid + "_" + startdt.toString("yyyyMMdd")
//                        + "_" + enddt.toString("yyyyMMdd") + ".txt";
//                QFile file(QDir(path).absoluteFilePath(fileName));
//                file.open(QIODevice::WriteOnly);
//                QTextStream out(&file);
//                out.setFieldWidth(24);
//                out.setFieldAlignment(QTextStream::AlignRight);

//                ProcessSeries series;
//                series.setSTCD(sid);
//                series.setStartDateTime(startdt);
//                series.setEndDateTime(enddt);

//                auto stageMap = series.getStageMap().toStdMap();
//                auto dischargeMap = series.getDischargeMap().toStdMap();
//                auto sedimentMap = series.getSedimentMap().toStdMap();

//                std::map<QDateTime, DGSDSProecssNode> map;

//                for (const auto &kv: stageMap)
//                {
//                    if (progress.wasCanceled())
//                    {
//                        file.close();
//                        return;
//                    }
//                    progress.setValue(base + (kv.first.toMSecsSinceEpoch() - startdt.toMSecsSinceEpoch())/1000);
//                    auto key = kv.first;
//                    if (map.find(key) != map.end())
//                    {
//                        map[key].stage = kv.second;
//                    }
//                    else
//                    {
//                        DGSDSProecssNode node;
//                        node.stage = kv.second;
//                        map.insert({key, node});
//                    }
//                }
//                for (const auto &kv: dischargeMap)
//                {
//                    if (progress.wasCanceled())
//                    {
//                        file.close();
//                        return;
//                    }
//                    progress.setValue(base + secs + (kv.first.toMSecsSinceEpoch() - startdt.toMSecsSinceEpoch())/1000);
//                    auto key = kv.first;
//                    if (map.find(key) != map.end())
//                    {
//                        map[key].discharge = kv.second;
//                    }
//                    else
//                    {
//                        DGSDSProecssNode node;
//                        node.discharge = kv.second;
//                        map.insert({key, node});
//                    }
//                }
//                for (const auto &kv: sedimentMap)
//                {
//                    if (progress.wasCanceled())
//                    {
//                        file.close();
//                        return;
//                    }
//                    progress.setValue(base + 2*secs + (kv.first.toMSecsSinceEpoch() - startdt.toMSecsSinceEpoch())/1000);
//                    auto key = kv.first;
//                    if (map.find(key) != map.end())
//                    {
//                        map[key].sediment = kv.second;
//                    }
//                    else
//                    {
//                        DGSDSProecssNode node;
//                        node.sediment = kv.second;
//                        map.insert({key, node});
//                    }
//                }

//                for (const auto &kv: map)
//                {
//                    if (progress.wasCanceled())
//                    {
//                        file.close();
//                        return;
//                    }
//                    progress.setValue(base + 3*secs + (kv.first.toMSecsSinceEpoch() - startdt.toMSecsSinceEpoch())/1000);
//                    out << kv.first.toString("yyyy-MM-dd hh:mm:ss")
//                        << kv.second.stage
//                        << kv.second.discharge
//                        << kv.second.sediment
//                        << "\r\n";
//                }
//                file.close();
//            }

//            progress.setValue(allsecs);
//            QDesktopServices::openUrl(QUrl::fromLocalFile(path));
//        }

//    });
    //    _generate->setMenu(generateMenu);
}

// update context
void ProcessViewer::showEvent(QShowEvent *event)
{
    this->updateContext();
}

void ProcessViewer::updateContext()
{
    if (g_dataProvider) {
        auto list = g_dataProvider->zqs_process_year_list();
        if (list.count() > 0) {
            _sb_selectYear->setMinimum(list.front());
            _sb_selectYear->setMaximum(list.back());
            Q_EMIT _pb_selectMonth->click();
        }
    }
}



void ProcessViewer::addViewToSplitter(QSplitter *splitter, ProcessChart *chart)
{
    // fix bug of not setting start/end datetime when init.
    chart->setStartDateTime(viewStartDateTime());
    chart->setEndDateTime(viewEndDateTime());

    // connect selection changing event to each other with the help of this.
    connect(chart, SIGNAL(selectionChanging(qreal,qreal)),
            this, SIGNAL(selectionChanging(qreal,qreal)));
    connect(this, SIGNAL(selectionChanging(qreal,qreal)),
            chart, SLOT(changeSelection(qreal,qreal)));
    // connect selection changed event to each other with the help of this.
    connect(chart, SIGNAL(selectionChanged(qreal,qreal)),
            this, SIGNAL(selectionChanged(qreal,qreal)));
    connect(this, SIGNAL(selectionChanged(qreal,qreal)),
            chart, SLOT(changeSelection(qreal,qreal)));

    auto gridWidget = new QWidget(splitter);
    gridWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(gridWidget, &QWidget::customContextMenuRequested,
            [=](const QPoint &pos) {
        auto menu = new QMenu(gridWidget);
        auto closeAction = menu->addAction(tr("关闭"));
        connect(closeAction, &QAction::triggered, [=](){
//            gridWidget->hide();
            gridWidget->setParent(nullptr);
            gridWidget->deleteLater();
        });
        menu->exec(gridWidget->mapToGlobal(pos));
    });

    auto grid = new QGridLayout(gridWidget);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);
    auto grid2Widget = new QWidget(gridWidget);
    grid2Widget->setFixedWidth(200);
    auto grid2 = new QGridLayout(grid2Widget);
    grid2->setContentsMargins(0, 0, 0, 0);

    QMap<QStandardItem *, QStandardItem *> stationColorMap;

    auto generatStationSelectorModel = [this, &stationColorMap](ProcessChart *chart)
    {
        auto model = new QStandardItemModel(this);

        model->setHorizontalHeaderItem(0, new QStandardItem(tr("流域/水系/测站")));
        model->setHorizontalHeaderItem(1, new QStandardItem(tr("线色")));
        QStandardItem *parentItem = model->invisibleRootItem();


        QStringList list;

        if (g_dataProvider) {
            list = g_dataProvider->zq_process_stcd_list();
        }


        for (const auto &stcd : list)
        {
            Station station(stcd);
            auto *stationItem = new QStandardItem(station.getDisplayName());
            stationItem->setData(stcd);
            stationItem->setCheckable(true);
            stationItem->setEditable(false);
            auto *colorItem = new QStandardItem();
            colorItem->setSelectable(false);
            colorItem->setCheckable(false);
            stationColorMap[stationItem] = colorItem;
            parentItem->appendRow(
                        QList<QStandardItem *>() << stationItem << colorItem);
        }

        connect(model, &QStandardItemModel::itemChanged, [=](QStandardItem* item) {
            if (item->hasChildren())
            {
                if (item->checkState() == Qt::Checked)
                {
                    auto rowCount = item->rowCount();
                    for (auto i = 0; i < rowCount; ++i)
                    {
                        if (item->child(i)->checkState() != Qt::Checked)
                        {
                            item->child(i)->setCheckState(Qt::Checked);
                        }
                    }
                }
                else if (item->checkState() == Qt::Unchecked)
                {
                    auto rowCount = item->rowCount();
                    for (auto i = 0; i < rowCount; ++i)
                    {
                        if (item->child(i)->checkState() != Qt::Unchecked)
                        {
                            item->child(i)->setCheckState(Qt::Unchecked);
                        }
                    }
                }
            }
            else // add or remove
            {
                auto stcd = item->data().toString();

                if (item->checkState() == Qt::Checked)
                {
                    auto series = std::make_shared<ProcessSeries>();

                    if (g_dataProvider) {
                        series->setDataProvider(g_dataProvider);
                    }

                    series->setSTCD(stcd);
                    QList<int> list;
                    auto color = ColorHelper::random();
                    auto colorItem = stationColorMap[item];
                    if (colorItem)
                    {
                        colorItem->setData(color, Qt::BackgroundRole);
                    }
                    series->setLineColor(color);
                    chart->addSeries(stcd, series);
                    chart->setStartDateTime(viewStartDateTime());
                    chart->setEndDateTime(viewEndDateTime());
                    chart->update();

                }
                else // unchecked
                {
                    chart->removeSeries(stcd);
                    chart->update();
                }
            }

            QStandardItem* parent = item->parent();
            while (parent != nullptr)
            {
                auto checkCount = 0;
                auto rowCount = parent->rowCount();
                bool hasPartiallyCheckedChild = false;

                for (auto i = 0; i < rowCount; ++i)
                {
                    if (parent->child(i)->checkState() == Qt::Checked)
                    {
                        checkCount++;
                    }
                    else if (parent->child(i)->checkState() == Qt::PartiallyChecked)
                    {
                        hasPartiallyCheckedChild = true;
                    }
                }

                if (hasPartiallyCheckedChild)
                {
                    parent->setCheckState(Qt::PartiallyChecked);
                }
                else if (0 == checkCount)
                {
                    parent->setCheckState(Qt::Unchecked);
                }
                else if (rowCount == checkCount)
                {
                    parent->setCheckState(Qt::Checked);
                }
                parent = parent->parent();
            }
        });
        return model;
    };

    auto treeView = new QTreeView(grid2Widget);
    treeView->setModel(generatStationSelectorModel(chart));
//    treeView->setColumnWidth(0, 110);
    auto delegate = new ColorDelegate(treeView);
    treeView->setItemDelegateForColumn(1, delegate);
    treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeView->setColumnWidth(1, 20);
//    treeView->setFixedWidth(150);

    grid2->addWidget(treeView, 1, 0);

    auto yAxis = new ProcessYAxisWidget(gridWidget);
    yAxis->setFixedWidth(50);
    yAxis->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    auto view = new View(chart, gridWidget);
    connect(view, &View::mouseMovePosX,
            _xAxis1, &ProcessXAxisWidget::mouseMovePosX);
    connect(view, &View::mouseMovePosY,
            yAxis, &ProcessYAxisWidget::mouseMovePosY);
    connect(chart, &ProcessChart::yUnitChanged,
            yAxis, &ProcessYAxisWidget::unitChange);

    grid->addWidget(grid2Widget, 0, 0);
    grid->addWidget(yAxis, 0, 1);
    grid->addWidget(view, 0, 2);

    splitter->addWidget(gridWidget);

    _views.append(view);
    _processCharts.append(chart);
}

QDateTime ProcessViewer::viewStartDateTime() const
{
    if (_processCharts.length() > 0) {
        return _processCharts[0]->startDateTime();
    }
    return _dte_startDateTime->dateTime();
}

QDateTime ProcessViewer::viewEndDateTime() const
{
    if (_processCharts.length() > 0) {
        return _processCharts[0]->endDateTime();
    }
    return _dte_endDateTime->dateTime();
}
