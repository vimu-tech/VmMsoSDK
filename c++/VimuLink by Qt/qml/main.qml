import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Vimu.Qml.Ctrls 1.0

ApplicationWindow {
    id: mianwindow
    width: 1280
    height: 720
    visible: true
    title: qsTr("VimuLink by Qt")

    property int oscBarWidth: 120

    /*Platform.MenuBar {
        Platform.Menu {
            title: qsTr("&File")

            Platform.MenuItem {
                text: qsTr("&Open")
                onTriggered: openDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("&Save As...")
                onTriggered: saveDialog.open()
            }
            Platform.MenuItem {
                text: qsTr("&Quit")
                onTriggered: close()
            }
        }
    }*/

    ///////////////////////////////////////////////////////////////
    Dds {
        id:dds1
        width: parent.width/3
        height: parent.height*2/3
        ddsChannelIndex: 0
    }

    Dds {
        id:dds2
        width: parent.width/3
        height: parent.height*2/3
        ddsChannelIndex: 1
    }

    Io {
        id:io
        width: parent.width*2/3
        height: parent.height*2/3
    }

    FFT {
        id:fft
        width: parent.width/3
        height: parent.height*2/3
        usbModel: vmusbwave
    }

    Filter {
        id:filter
        width: parent.width/3
        height: parent.height*2/3
    }

    Power {
        id:power
        width: parent.width*2/3
        height: parent.height*2/3
        usbModel: vmusbwave
    }

    Meter {
        id:meter
        width: parent.width*2/3
        height: parent.height*2/3
        usbModel: vmusbwave
    }
    ///////////////////////////////////////////////////////////////
    VmUsbWave {
        id: vmusbwave
        objectName: "objectvmusbwave"

        onDataUpdated: {
            vmdsoplot.redraw();

            if(vmusbwave.workMode==0)
                vmusbwave.readyNextCapture();
        }
    }

    FFTCal {
        id: fftCal
        objectName: "objectfft"
    }

    FilterCal {
        id: filterCal
        objectName: "objectfilter"
    }

    Action {
        id: plotCh1AxisAction

        property double interal: 0;
        onTriggered:{
            interal = vmdsoplot.getAxisInterval(axisYSpinCh1.value);
            vmdsoplot.setYAxisRange("CH1", (-5-axisYSliderCh1.value)*interal, interal);
        }
    }

    Action {
        id: plotCh2AxisAction

        property double interal: 0;
        onTriggered:{
            interal = vmdsoplot.getAxisInterval(axisYSpinCh2.value);
            vmdsoplot.setYAxisRange("CH2", (-5-axisYSliderCh2.value)*interal, interal);
        }
    }

    Action {
        id: plotCh3AxisAction

        property double interal: 0;
        onTriggered:{
            interal = vmdsoplot.getAxisInterval(axisYSpinCh3.value);
            vmdsoplot.setYAxisRange("CH3", (-5-axisYSliderCh3.value)*interal, interal);
        }
    }

    Action {
        id: plotCh4AxisAction

        property double interal: 0;
        onTriggered:{
            interal = vmdsoplot.getAxisInterval(axisYSpinCh4.value);
            vmdsoplot.setYAxisRange("CH4", (-5-axisYSliderCh4.value)*interal, interal);
        }
    }

    Action {
        id: plotTimeAxisAction

        property double interal: 0;
        onTriggered:{
            interal = vmdsoplot.getTimeAxisInterval(axisTimeSpin.value);

            if(vmusbwave.workMode==0)
                vmdsoplot.setTimeAxisRange((-5-axisTimeSlider.value)*interal, interal);
            else
                vmdsoplot.setTimeAxisRange((-axisTimeSlider.value)*interal, interal);
        }
    }

    RowLayout {
            anchors.fill: parent
            spacing: 0  // 确保没有额外的间距

            ColumnLayout {
                id: functionbar
                Layout.fillHeight: true
                Layout.preferredWidth: 68

                ColumnLayout {
                   Layout.margins: 4  // 统一设置四个方向的外边距
                   spacing: 4  // 设置控件之间的间距

                   Button {
                       id:dds1Btn
                       visible: vmusbwave.ddsChannelNum>0
                       Layout.preferredWidth: 60  // 设置正方形边长
                       Layout.preferredHeight: 60 // 设置为相同值以形成正方形

                       // 文本居中设置
                       contentItem: Text {
                           text: dds1Btn.text
                           font: dds1Btn.font
                           color: dds1Btn.down ? "#cccccc" : "#ffffff"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                       }

                       background: Rectangle {
                           implicitWidth: 60
                           implicitHeight: 60
                           color: dds1Btn.down ? "#555555" : "#444444"
                           border.color: "#666666"
                           border.width: 1
                           radius: 4
                       }

                       text: "DDS1"

                       onClicked: {
                           dds1.switchDlg();
                       }
                   }

                   Button {
                       id:dds2Btn
                       visible: vmusbwave.ddsChannelNum>1
                       Layout.preferredWidth: 60  // 设置正方形边长
                       Layout.preferredHeight: 60 // 设置为相同值以形成正方形

                       // 文本居中设置
                       contentItem: Text {
                           text: dds2Btn.text
                           font: dds2Btn.font
                           color: dds2Btn.down ? "#cccccc" : "#ffffff"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                       }

                       background: Rectangle {
                           implicitWidth: 60
                           implicitHeight: 60
                           color: dds2Btn.down ? "#555555" : "#444444"
                           border.color: "#666666"
                           border.width: 1
                           radius: 4
                       }

                       text: "DDS2"

                       onClicked: {
                           dds2.switchDlg();
                       }
                   }

                   Button {
                       id:ioBtn
                       Layout.preferredWidth: 60  // 设置正方形边长
                       Layout.preferredHeight: 60 // 设置为相同值以形成正方形

                       // 文本居中设置
                       contentItem: Text {
                           text: ioBtn.text
                           font: ioBtn.font
                           color: ioBtn.down ? "#cccccc" : "#ffffff"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                       }

                       background: Rectangle {
                           implicitWidth: 60
                           implicitHeight: 60
                           color: ioBtn.down ? "#555555" : "#444444"
                           border.color: "#666666"
                           border.width: 1
                           radius: 4
                       }

                       text: " IOs "

                       onClicked: {
                           io.switchDlg();
                       }
                   }

                   Button {
                       id:powerBtn
                       Layout.preferredWidth: 60  // 设置正方形边长
                       Layout.preferredHeight: 60 // 设置为相同值以形成正方形

                       // 文本居中设置
                       contentItem: Text {
                           text: powerBtn.text
                           font: powerBtn.font
                           color: powerBtn.down ? "#cccccc" : "#ffffff"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                       }

                       background: Rectangle {
                           implicitWidth: 60
                           implicitHeight: 60
                           color: powerBtn.down ? "#555555" : "#444444"
                           border.color: "#666666"
                           border.width: 1
                           radius: 4
                       }

                       text: "Power"

                       onClicked: {
                           power.switchDlg();
                       }
                   }

                   Button {
                       id:meterBtn
                       Layout.preferredWidth: 60  // 设置正方形边长
                       Layout.preferredHeight: 60 // 设置为相同值以形成正方形

                       // 文本居中设置
                       contentItem: Text {
                           text: meterBtn.text
                           font: meterBtn.font
                           color: meterBtn.down ? "#cccccc" : "#ffffff"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                       }

                       background: Rectangle {
                           implicitWidth: 60
                           implicitHeight: 60
                           color: meterBtn.down ? "#555555" : "#444444"
                           border.color: "#666666"
                           border.width: 1
                           radius: 4
                       }

                       text: "Meter"

                       onClicked: {
                           meter.switchDlg();
                       }
                   }

                   Rectangle{
                       Layout.preferredWidth: 52  // 设置正方形边长
                       Layout.preferredHeight: 2 // 设置为相同值以形成正方形
                       color: "white"
                       Layout.margins: 4
                   }

                   Button {
                       id:filterBtn
                       Layout.preferredWidth: 60  // 设置正方形边长
                       Layout.preferredHeight: 60 // 设置为相同值以形成正方形

                       // 文本居中设置
                       contentItem: Text {
                           text: filterBtn.text
                           font: filterBtn.font
                           color: filterBtn.down ? "#cccccc" : "#ffffff"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                       }

                       background: Rectangle {
                           implicitWidth: 60
                           implicitHeight: 60
                           color: filterBtn.down ? "#555555" : "#444444"
                           border.color: "#666666"
                           border.width: 1
                           radius: 4
                       }

                       text: "Filter"

                       onClicked: {
                           filter.switchDlg();
                       }
                   }

                   Button {
                       id:fftBtn
                       Layout.preferredWidth: 60  // 设置正方形边长
                       Layout.preferredHeight: 60 // 设置为相同值以形成正方形

                       // 文本居中设置
                       contentItem: Text {
                           text: fftBtn.text
                           font: fftBtn.font
                           color: fftBtn.down ? "#cccccc" : "#ffffff"
                           horizontalAlignment: Text.AlignHCenter
                           verticalAlignment: Text.AlignVCenter
                           elide: Text.ElideRight
                       }

                       background: Rectangle {
                           implicitWidth: 60
                           implicitHeight: 60
                           color: fftBtn.down ? "#555555" : "#444444"
                           border.color: "#666666"
                           border.width: 1
                           radius: 4
                       }

                       text: " FFT "

                       onClicked: {
                           fft.switchDlg();
                       }
                   }

                   // 底部填充
                   Item {
                       Layout.fillHeight: true
                   }
               }
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                VmSimplePlot {
                    id: vmdsoplot
                    objectName: "objectvmdsoplot"
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    property double interal: 0;

                    //plot加载完成，初始化通道和时基
                    Component.onCompleted: {
                        vmdsoplot.addAxis("CH1", "#FFFF00");
                        plotCh1AxisAction.trigger()

                        vmdsoplot.addAxis("CH2", "#FF0000");
                        plotCh2AxisAction.trigger()

                        vmdsoplot.addAxis("CH3", "#FF00FF");
                        plotCh3AxisAction.trigger()

                        vmdsoplot.addAxis("CH4", "#00FFFF");
                        plotCh4AxisAction.trigger()

                        plotTimeAxisAction.trigger()

                        vmdsoplot.computeAxisPos();
                        vmdsoplot.redraw();
                    }

                    onYAxisRangeChanged: function (trackname, anArray) {
                        if(trackname==="CH1")
                            vmusbwave.setPlotRangeCh1(anArray);
                        else if(trackname==="CH2")
                            vmusbwave.setPlotRangeCh2(anArray);
                        else if(trackname==="CH3")
                            vmusbwave.setPlotRangeCh3(anArray);
                        else if(trackname==="CH4")
                            vmusbwave.setPlotRangeCh4(anArray);
                    }
                }
            }

            ColumnLayout {
                id: oscbar
                Layout.fillHeight: true
                Layout.preferredWidth: oscBarWidth
                Layout.margins: 4  // 统一设置四个方向的外边距

                ScrollView {
                    id: scrollView
                    //Layout.fillWidth: true
                    width: parent.width
                    Layout.fillHeight: true

                    ScrollBar.vertical.policy: ScrollBar.AsNeeded
                    ScrollBar.vertical.width: 6 // 设置滚动条宽度

                    ColumnLayout {
                        id: oscBarContent
                        Layout.fillHeight: true
                        spacing: 4

                        GroupBox {
                            title: "Work Mode"
                            Layout.fillWidth: true

                            ColumnLayout  {
                                width: parent.width
                                enabled: !vmusbwave.captureEnable

                                ComboBox {
                                    id: workModeCombox
                                    Layout.fillWidth: true
                                    model: ["Real-time mode", "Stream mode"]
                                    currentIndex: vmusbwave.workMode

                                    onActivated: {
                                        console.log("workModeCombox " + currentText);
                                        vmusbwave.workMode = currentIndex

                                        if(currentIndex==0)
                                            axisTimeSpin.value = 12
                                        else if(currentIndex==1)
                                            axisTimeSpin.value = 24 //将时间调整为1秒，每格，方便查看采集过程
                                         plotTimeAxisAction.trigger()
                                    }
                                }
                            }
                        }

                        GroupBox {
                            //title: "CH1"
                            visible: vmusbwave.oscChannelNum>0
                            enabled: (vmusbwave.workMode==0) || (vmusbwave.workMode==1&&!vmusbwave.captureEnable)
                            Layout.fillWidth: true

                            ColumnLayout  {
                                width: parent.width

                                RowLayout{
                                    Layout.fillWidth: true

                                    CheckBox {
                                        id: enCh1
                                        text: "CH1"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.oscCheckCh1
                                        onClicked: vmusbwave.oscCheckCh1 = checked
                                    }

                                    Item{
                                        Layout.fillWidth: true
                                    }

                                    CheckBox {
                                        id: acCh1
                                        text: "AC"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.acEnableCh1
                                        onClicked: vmusbwave.acEnableCh1 = checked
                                    }
                                }


                                SpinBox{
                                    id: axisYSpinCh1
                                    Layout.fillWidth: true
                                    from: 0
                                    to: vmdsoplot.axisIntervalList.length-1
                                    value: 6

                                    textFromValue: function(value) {
                                        return vmdsoplot.axisIntervalList[value];
                                    }

                                    valueFromText: function(text) {
                                        for (var i = 0; i < vmdsoplot.axisIntervalList.length; ++i) {
                                            if (vmdsoplot.axisIntervalList[i]===text)
                                                return i
                                        }
                                        return value
                                    }

                                    onValueChanged: {
                                        plotCh1AxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                }

                                Slider {
                                    id: axisYSliderCh1
                                    Layout.fillWidth: true
                                    from: -5
                                    value: 1
                                    to: 5

                                    property double interal: 0;
                                    onValueChanged: {
                                        plotCh1AxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                }
                            }
                        }

                        GroupBox {
                            //title: "CH2"
                            visible: vmusbwave.oscChannelNum>1
                            Layout.fillWidth: true

                            ColumnLayout  {
                                width: parent.width

                                RowLayout{
                                    Layout.fillWidth: true

                                    CheckBox {
                                        id: enCh2
                                        text: "CH2"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.oscCheckCh2
                                        onClicked: vmusbwave.oscCheckCh2 = checked
                                    }

                                    Item{
                                        Layout.fillWidth: true
                                    }

                                    CheckBox {
                                        id: acCh2
                                        text: "AC"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.acEnableCh2
                                        onClicked: vmusbwave.acEnableCh2 = checked
                                    }
                                }

                                SpinBox{
                                    id: axisYSpinCh2
                                    Layout.fillWidth: true
                                    from: 0
                                    to: vmdsoplot.axisIntervalList.length-1
                                    value: 6

                                    textFromValue: function(value) {
                                        return vmdsoplot.axisIntervalList[value];
                                    }

                                    valueFromText: function(text) {
                                        for (var i = 0; i < vmdsoplot.axisIntervalList.length; ++i) {
                                            if (vmdsoplot.axisIntervalList[i]===text)
                                                return i
                                        }
                                        return value
                                    }

                                    property double interal: 0;
                                    onValueChanged: {
                                        plotCh2AxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                }

                                Slider {
                                    id: axisYSliderCh2
                                    Layout.fillWidth: true
                                    from: -5
                                    value: -1
                                    to: 5

                                    onValueChanged: {
                                        plotCh2AxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                }
                            }
                        }

                        GroupBox {
                            //title: "CH3"
                            visible: vmusbwave.oscChannelNum>2
                            Layout.fillWidth: true

                            ColumnLayout  {
                                width: parent.width

                                RowLayout{
                                    Layout.fillWidth: true

                                    CheckBox {
                                        id: enCh3
                                        text: "CH3"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.oscCheckCh3
                                        onClicked: vmusbwave.oscCheckCh3 = checked
                                    }

                                    Item{
                                        Layout.fillWidth: true
                                    }

                                    CheckBox {
                                        id: acCh3
                                        text: "AC"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.acEnableCh3
                                        onClicked: vmusbwave.acEnableCh3 = checked
                                    }
                                }

                                SpinBox{
                                    id: axisYSpinCh3
                                    Layout.fillWidth: true
                                    from: 0
                                    to: vmdsoplot.axisIntervalList.length-1
                                    value: 6

                                    textFromValue: function(value) {
                                        return vmdsoplot.axisIntervalList[value];
                                    }

                                    valueFromText: function(text) {
                                        for (var i = 0; i < vmdsoplot.axisIntervalList.length; ++i) {
                                            if (vmdsoplot.axisIntervalList[i]===text)
                                                return i
                                        }
                                        return value
                                    }

                                    property double interal: 0;
                                    onValueChanged: {
                                        plotCh3AxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                }

                                Slider {
                                    id: axisYSliderCh3
                                    Layout.fillWidth: true
                                    from: -5
                                    value: 2
                                    to: 5

                                    onValueChanged: {
                                        plotCh3AxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                } 
                            }
                        }

                        GroupBox {
                            //title: "CH4"
                            visible: vmusbwave.oscChannelNum>3
                            Layout.fillWidth: true

                            ColumnLayout  {
                                width: parent.width

                                RowLayout{
                                    Layout.fillWidth: true

                                    CheckBox {
                                        id: enCh4
                                        text: "CH4"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.oscCheckCh4
                                        onClicked: vmusbwave.oscCheckCh4 = checked
                                    }

                                    Item{
                                        Layout.fillWidth: true
                                    }

                                    CheckBox {
                                        id: acCh4
                                        text: "AC"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.acEnableCh4
                                        onClicked: vmusbwave.acEnableCh4 = checked
                                    }
                                }

                                SpinBox{
                                    id: axisYSpinCh4
                                    Layout.fillWidth: true
                                    from: 0
                                    to: vmdsoplot.axisIntervalList.length-1
                                    value: 6

                                    textFromValue: function(value) {
                                        return vmdsoplot.axisIntervalList[value];
                                    }

                                    valueFromText: function(text) {
                                        for (var i = 0; i < vmdsoplot.axisIntervalList.length; ++i) {
                                            if (vmdsoplot.axisIntervalList[i]===text)
                                                return i
                                        }
                                        return value
                                    }

                                    onValueChanged: {
                                        plotCh4AxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                }

                                Slider {
                                    id: axisYSliderCh4
                                    Layout.fillWidth: true
                                    from: -5
                                    value: -2
                                    to: 5

                                    property double interal: 0;
                                    onValueChanged: {
                                        plotCh4AxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                } 
                            }
                        }

                        GroupBox {
                            title: "Time"
                            Layout.fillWidth: true

                            ColumnLayout  {
                                width: parent.width

                                SpinBox{
                                    id: axisTimeSpin
                                    Layout.fillWidth: true
                                    from: 0
                                    to: vmdsoplot.timeAxisIntervalList.length-1
                                    value: 12

                                    textFromValue: function(value) {
                                        return vmdsoplot.timeAxisIntervalList[value];
                                    }

                                    valueFromText: function(text) {
                                        for (var i = 0; i < vmdsoplot.timeAxisIntervalList.length; ++i) {
                                            if (vmdsoplot.timeAxisIntervalList[i]===text)
                                                return i
                                        }
                                        return value
                                    }

                                    onValueChanged: {
                                        plotTimeAxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                }

                                Slider {
                                    id: axisTimeSlider
                                    Layout.fillWidth: true
                                    from: -5
                                    value: 0
                                    to: 5

                                    property double interal: 0;
                                    onValueChanged: {
                                        plotTimeAxisAction.trigger()
                                        vmdsoplot.redraw();
                                    }
                                }
                            }
                        }

                        GroupBox {
                            id: triggerCtrl
                            Layout.fillWidth: true
                            padding: 4
                            spacing: 4
                            title: qsTr("Trigger")
                            visible: vmusbwave.workMode==0

                            ColumnLayout  {
                                width: parent.width

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 4

                                    Label {
                                        text: "Trigger Mode"
                                    }

                                    ComboBox {
                                        id: triggerModeCombox
                                        Layout.fillWidth: true
                                        model:["Auto","Normal"]
                                        currentIndex: vmusbwave.triggerMode

                                        onActivated: {
                                            console.log("triggerModeCombox " + currentText);
                                            vmusbwave.triggerMode = triggerModeCombox.currentIndex;
                                        }
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: "Trigger Style"
                                    }

                                    ComboBox {
                                        id: triggerStyleCombox
                                        Layout.fillWidth: true
                                        model:["Not trigger","Rising edge","Falling edge","Edge",
                                            "Positive Pulse width(>)","Positive Pulse width(<)","Positive Pulse width(<>)",
                                            "Negative Pulse width(>)","Negative Pulse width(<)","Negative Pulse width(<>)"]
                                        currentIndex: vmusbwave.triggerStyle

                                        onActivated: {
                                            console.log("triggerStyleCombox " + currentText);
                                            vmusbwave.triggerStyle = triggerStyleCombox.currentIndex;
                                        }
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: "Trigger Source"
                                    }

                                    ComboBox {
                                        id: triggerSourceCombox
                                        Layout.fillWidth: true
                                        model: vmusbwave.triggerSourceList
                                        currentIndex: vmusbwave.triggerSourceIndex

                                        onActivated: {
                                            console.log("triggerSourceCombox " + currentText);
                                            vmusbwave.triggerSourceIndex = currentIndex;
                                        }
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: "Trigger Level(mV)"
                                    }

                                    TextField {
                                        id: triggerLevelEdit
                                        Layout.fillWidth: true
                                        validator: IntValidator {
                                                        id: triggerLevelEditIntValidator
                                                        bottom: -5000;
                                                        top: 5000;
                                        }
                                        text: vmusbwave.triggerLevelmV

                                        onEditingFinished:{
                                            if(triggerSourceCombox.currentText == "CH1")
                                                vmusbwave.setTriggerLevelSense(Number(text), Number(triggerSenseEdit.text), vmdsoplot.getYAxisInterval("CH1")*1000);
                                            else if(triggerSourceCombox.currentText == "CH2")
                                                vmusbwave.setTriggerLevelSense(Number(text), Number(triggerSenseEdit.text), vmdsoplot.getYAxisInterval("CH2")*1000);
                                            else if(triggerSourceCombox.currentText == "CH3")
                                                vmusbwave.setTriggerLevelSense(Number(text), Number(triggerSenseEdit.text), vmdsoplot.getYAxisInterval("CH3")*1000);
                                            else if(triggerSourceCombox.currentText == "CH4")
                                                vmusbwave.setTriggerLevelSense(Number(text), Number(triggerSenseEdit.text), vmdsoplot.getYAxisInterval("CH4")*1000);
                                        }
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: "Trigger Sense"
                                    }

                                    TextField {
                                        id: triggerSenseEdit
                                        Layout.fillWidth: true
                                        validator: DoubleValidator {
                                                        id: triggerSenseEditIntValidator
                                                        bottom: 0.1;
                                                        top: 0.9;
                                        }
                                        text: vmusbwave.triggerSense

                                        onEditingFinished: {
                                            if(triggerSourceCombox.currentText == "CH1")
                                                vmusbwave.setTriggerLevelSense(Number(triggerLevelEdit.text), Number(triggerSenseEdit.text), vmdsoplot.getYAxisInterval("CH1")*1000);
                                            else if(triggerSourceCombox.currentText == "CH2")
                                                vmusbwave.setTriggerLevelSense(Number(triggerLevelEdit.text), Number(triggerSenseEdit.text), vmdsoplot.getYAxisInterval("CH2")*1000);
                                            else if(triggerSourceCombox.currentText == "CH3")
                                                vmusbwave.setTriggerLevelSense(Number(triggerLevelEdit.text), Number(triggerSenseEdit.text), vmdsoplot.getYAxisInterval("CH3")*1000);
                                            else if(triggerSourceCombox.currentText == "CH4")
                                                vmusbwave.setTriggerLevelSense(Number(triggerLevelEdit.text), Number(triggerSenseEdit.text), vmdsoplot.getYAxisInterval("CH4")*1000);
                                        }
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: "Pre Trigger Percent"
                                    }

                                    TextField {
                                        id: preTriggerEdit
                                        Layout.fillWidth: true
                                        validator: IntValidator {
                                                        id: preTriggerEditIntValidator
                                                        bottom: 5;
                                                        top: 95;
                                        }
                                        text: vmusbwave.preTriggerPercent

                                        onEditingFinished: vmusbwave.preTriggerPercent = Number(text)
                                    }
                                }
                            }
                        }

                        GroupBox{
                            Layout.fillWidth: true
                            padding: 4
                            spacing: 4

                            ColumnLayout  {
                                width: parent.width

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 4

                                    Label {
                                        text: "Sample Rate"
                                    }

                                    ComboBox {
                                        id: sampleCombox
                                        Layout.fillWidth: true
                                        model: vmusbwave.supportSamples
                                        currentIndex: vmusbwave.sampleIndex

                                        onActivated: {
                                            console.log("sampleCombox " + currentText);
                                            vmusbwave.sampleIndex = currentIndex
                                        }
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 4

                                    Label {
                                        text: "Length(KB)"
                                    }

                                    TextField {
                                        id: captureLengthText
                                        Layout.fillWidth: true
                                        validator: IntValidator {
                                                        id: captureLengthTextIntValidator
                                                        bottom: 0;
                                                        top: 64*1024;
                                        }
                                        text: vmusbwave.captureLength

                                        onEditingFinished: vmusbwave.captureLength = Number(text)
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Item { Layout.fillWidth: true }  // 左侧空白

                                    CheckBox {
                                        id: captureEn
                                        text: "Capture"
                                        focusPolicy: Qt.TabFocus
                                        checked: vmusbwave.captureEnable
                                        enabled: vmusbwave.devReday

                                        onClicked: vmusbwave.captureEnable = checked;
                                    }

                                    Item { Layout.fillWidth: true }  // 右侧空白
                                }
                            }
                        }
                    }
                }
            }
        }
}

