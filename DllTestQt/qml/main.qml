import QtQuick 2.12
import QtQuick.Controls 2.12
//import Qt.labs.platform 1.0 as Platform
import Vimu.Qml.Ctrls 1.0

ApplicationWindow {
    id: mianwindow
    width: 1000
    height: 600
    visible: true
    title: qsTr("VmMso")

    function usbDevCallBack(add_remove)
    {
        console.log("QML get message:",add_remove);
    }

    Action {
        id: openCh1Action
        //shortcut: StandardKey.Open
        onTriggered:{
            vmusbwave.qmlcallcpp_style1();   //
            vmusbwave.qmlcallcpp_style2(1);

            if(vmusbwave.isStar())
                vmusbwave.stop(1);
            else
                vmusbwave.star(1);
        }
    }

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

    ToolBar {
        id: toolbar_dds
        anchors.left: parent.left
       // anchors.right: parent.right
        anchors.top: parent.top
        //anchors.bottom: parent.bottom
        //anchors.margins: 4
        width: toolbar_io.width
        background: Rectangle {
                    color: "#333333"
                }

        Column {
            padding: 4
            spacing: 4
            width: parent.width

            GroupBox {
                id: ddsCtrl
                width: parent.width
                padding: 4
                spacing: 4
                anchors.horizontalCenter: parent.horizontalCenter
                title: qsTr("DDS")

                Column {
                    padding: 4
                    spacing: 4

                    Row {
                        id: ddsCtrlWave
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: ddsWave.verticalCenter
                            text: "Wave"
                        }

                        ComboBox {
                            id: ddsWave
                            model:["Sine","Square","Ramp","Noise","DC"]

                            onActivated: {
                                console.log("ddsWave " + currentText);
                                vmusbwave.setDdsWaveIndex(ddsWave.currentIndex);
                            }
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: ddsDutyEdit.verticalCenter
                            text: "Duty(%)"
                        }

                        TextField {
                            id: ddsDutyEdit
                            validator: IntValidator {
                                            id: ddsDutyEditIntValidator
                                            bottom: 5;
                                            top: 95;
                            }

                            onEditingFinished: vmusbwave.setDdsDuty(Number(text));
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: ddsFreqEdit.verticalCenter
                            text: "Freq(Hz)"
                        }

                        TextField {
                            id: ddsFreqEdit
                            validator: IntValidator {
                                            id: ddsFreqEditIntValidator
                                            bottom: 1;
                                            top: 2000000;
                            }

                            onEditingFinished: vmusbwave.setDdsFreq(Number(text));
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: ddsAmplitudeEdit.verticalCenter
                            text: "Amplitude(mV)"
                        }

                        TextField {
                            id: ddsAmplitudeEdit
                            validator: IntValidator {
                                            id: ddsAmplitudeEditIntValidator
                                            bottom: 10;
                                            top: 6000;
                            }

                            onEditingFinished: vmusbwave.setDdsAmplitudeMv(Number(text));
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: ddsBiasEdit.verticalCenter
                            text: "Bias(mV)"
                        }

                        TextField {
                            id: ddsBiasEdit
                            validator: IntValidator {
                                            id: ddsBiasEditIntValidator
                                            bottom: -2000;
                                            top: 2000;
                            }

                            onEditingFinished: vmusbwave.setDdsBiasMv(Number(text));
                        }
                    }

                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter
                        CheckBox {
                            id: ddsEn
                            text: "Output"
                            focusPolicy: Qt.TabFocus
                            //enabled: textArea.selectedText
                            onClicked: vmusbwave.DdsOutputEnable(ddsEn.checked);
                        }
                    }
                }
            }
        }
    }

    ToolBar {
        id: toolbar_io
        anchors.left: parent.left
       // anchors.right: parent.right
        //anchors.top: parent.top
        anchors.bottom: parent.bottom
        //anchors.margins: 4
        background: Rectangle {
                    color: "#333333"
                }

        Column {
            padding: 4
            spacing: 4
            width: parent.width

            GroupBox {
                id: ioCtrl
                padding: 4
                spacing: 4
                anchors.horizontalCenter: parent.horizontalCenter
                title: qsTr("IO")

                Column {
                    padding: 4
                    spacing: 4

                    Row {
                        id: io0Ctrl
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        CheckBox {
                            id: io0CtrlEn
                            text: "IO0"

                            onClicked:
                            {
                                if(io0CtrlEn.checked)
                                {
                                    vmusbwave.setIOInOut(0, io0InOut.currentIndex);
                                    if(io0InOut.currentIndex==1)
                                        vmusbwave.setIOOutState(0, io0OutState.currentIndex);
                                }
                                vmusbwave.setIOEnable(0, io0CtrlEn.checked);
                            }
                        }

                        ComboBox {
                            id: io0InOut
                            width:90
                            model:["Input","Output"]

                            onActivated: {
                                console.log("io0InOut " + currentText);
                                vmusbwave.setIOInOut(0, io0InOut.currentIndex);
                            }
                        }

                        ComboBox {
                            id: io0OutState
                            width:60
                            model:["0","1"]

                            onActivated: {
                                console.log("io0OutState " + currentText);
                                if(io0InOut.currentIndex==1)
                                    vmusbwave.setIOOutState(0, io0OutState.currentIndex);
                            }
                        }

                        Rectangle  {
                             id: io0InState
                             anchors.verticalCenter: io0InOut.verticalCenter
                             width: 30
                             height: 30
                             color: "green"
                        }
                    }

                    Row {
                        id: io1Ctrl
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        CheckBox {
                            id: io1CtrlEn
                            text: "IO1"

                            onClicked:
                            {
                                if(io1CtrlEn.checked)
                                {
                                    vmusbwave.setIOInOut(1, io1InOut.currentIndex);
                                    if(io1InOut.currentIndex==1)
                                        vmusbwave.setIOOutState(1, io1OutState.currentIndex);
                                }
                                vmusbwave.setIOEnable(1, io1CtrlEn.checked);
                            }
                        }

                        ComboBox {
                            id: io1InOut
                            width:90
                            model:["Input","Output"]

                            onActivated: {
                                console.log("io1InOut " + currentText);
                                vmusbwave.setIOInOut(1, io1InOut.currentIndex);
                            }
                        }

                        ComboBox {
                            id: io1OutState
                            width:60
                            model:["0","1"]

                            onActivated: {
                                console.log("io1OutState " + currentText);
                                if(io1InOut.currentIndex==1)
                                    vmusbwave.setIOOutState(1, io1OutState.currentIndex);
                            }
                        }

                        Rectangle  {
                             id: io1InState
                             anchors.verticalCenter: io1InOut.verticalCenter
                             width: 30
                             height: 30
                             color: "green"
                        }
                    }

                    Row {
                        id: io2Ctrl
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        CheckBox {
                            id: io2CtrlEn
                            text: "IO2"

                            onClicked:
                            {
                                if(io2CtrlEn.checked)
                                {
                                    vmusbwave.setIOInOut(2, io2InOut.currentIndex);
                                    if(io2InOut.currentIndex==1)
                                        vmusbwave.setIOOutState(2, io2OutState.currentIndex);
                                }
                                vmusbwave.setIOEnable(2, io2CtrlEn.checked);
                            }
                        }

                        ComboBox {
                            id: io2InOut
                            width:90
                            model:["Input","Output"]

                            onActivated: {
                                console.log("io2InOut " + currentText);
                                vmusbwave.setIOInOut(2, io2InOut.currentIndex);
                            }
                        }

                        ComboBox {
                            id: io2OutState
                            width:60
                            model:["0","1"]

                            onActivated: {
                                console.log("io2OutState " + currentText);
                                if(io2InOut.currentIndex==1)
                                    vmusbwave.setIOOutState(2, io2OutState.currentIndex);
                            }
                        }

                        Rectangle  {
                             id: io2InState
                             anchors.verticalCenter: io2InOut.verticalCenter
                             width: 30
                             height: 30
                             color: "green"
                        }
                    }

                    Row {
                        id: io3Ctrl
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        CheckBox {
                            id: io3CtrlEn
                            text: "IO3"

                            onClicked:
                            {
                                if(io3CtrlEn.checked)
                                {
                                    vmusbwave.setIOInOut(3, io3InOut.currentIndex);
                                    if(io3InOut.currentIndex==1)
                                        vmusbwave.setIOOutState(3, io3OutState.currentIndex);
                                }
                                vmusbwave.setIOEnable(3, io3CtrlEn.checked);
                            }
                        }

                        ComboBox {
                            id: io3InOut
                            width:90
                            model:["Input","Output"]

                            onActivated: {
                                console.log("io3InOut " + currentText);
                                vmusbwave.setIOInOut(3, io3InOut.currentIndex);
                            }
                        }

                        ComboBox {
                            id: io3OutState
                            width:60
                            model:["0","1"]

                            onActivated: {
                                console.log("io3OutState " + currentText);
                                if(io3InOut.currentIndex==1)
                                    vmusbwave.setIOOutState(3, io3OutState.currentIndex);
                            }
                        }

                        Rectangle  {
                             id: io3InState
                             anchors.verticalCenter: io3InOut.verticalCenter
                             width: 30
                             height: 30
                             color: "green"
                        }
                    }

                    Row {
                        id: io4Ctrl
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        CheckBox {
                            id: io4CtrlEn
                            text: "IO4"

                            onClicked:
                            {
                                if(io4CtrlEn.checked)
                                {
                                    vmusbwave.setIOInOut(4, io4InOut.currentIndex);
                                    if(io4InOut.currentIndex==1)
                                        vmusbwave.setIOOutState(4, io4OutState.currentIndex);
                                }
                                vmusbwave.setIOEnable(4, io4CtrlEn.checked);
                            }
                        }

                        ComboBox {
                            id: io4InOut
                            width:90
                            model:["Input","Output"]

                            onActivated: {
                                console.log("io4InOut " + currentText);
                                vmusbwave.setIOInOut(4, io4InOut.currentIndex);
                            }
                        }

                        ComboBox {
                            id: io4OutState
                            width:60
                            model:["0","1"]

                            onActivated: {
                                console.log("io4OutState " + currentText);
                                if(io4InOut.currentIndex==1)
                                    vmusbwave.setIOOutState(4, io4OutState.currentIndex);
                            }
                        }

                        Rectangle  {
                             id: io4InState
                             anchors.verticalCenter: io4InOut.verticalCenter
                             width: 30
                             height: 30
                             color: "green"
                        }
                    }

                    Row {
                        id: io5Ctrl
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        CheckBox {
                            id: io5CtrlEn
                            text: "IO5"

                            onClicked:
                            {
                                if(io5CtrlEn.checked)
                                {
                                    vmusbwave.setIOInOut(5, io5InOut.currentIndex);
                                    if(io5InOut.currentIndex==1)
                                        vmusbwave.setIOOutState(5, io5OutState.currentIndex);
                                }
                                vmusbwave.setIOEnable(5, io5CtrlEn.checked);
                            }
                        }

                        ComboBox {
                            id: io5InOut
                            width:90
                            model:["Input","Output"]

                            onActivated: {
                                console.log("io5InOut " + currentText);
                                vmusbwave.setIOInOut(5, io5InOut.currentIndex);
                            }
                        }

                        ComboBox {
                            id: io5OutState
                            width:60
                            model:["0","1"]

                            onActivated: {
                                console.log("io5OutState " + currentText);
                                if(io5InOut.currentIndex==1)
                                    vmusbwave.setIOOutState(5, io5OutState.currentIndex);
                            }
                        }

                        Rectangle  {
                             id: io5InState
                             anchors.verticalCenter: io5InOut.verticalCenter
                             width: 30
                             height: 30
                             color: "green"
                        }
                    }

                    Row {
                        id: io6Ctrl
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        CheckBox {
                            id: io6CtrlEn
                            text: "IO6"

                            onClicked:
                            {
                                if(io6CtrlEn.checked)
                                {
                                    vmusbwave.setIOInOut(6, io6InOut.currentIndex);
                                    if(io6InOut.currentIndex==1)
                                        vmusbwave.setIOOutState(6, io6OutState.currentIndex);
                                }
                                vmusbwave.setIOEnable(6, io6CtrlEn.checked);
                            }
                        }

                        ComboBox {
                            id: io6InOut
                            width:90
                            model:["Input","Output"]

                            onActivated: {
                                console.log("io6InOut " + currentText);
                                vmusbwave.setIOInOut(6, io6InOut.currentIndex);
                            }
                        }

                        ComboBox {
                            id: io6OutState
                            width:60
                            model:["0","1"]

                            onActivated: {
                                console.log("io6OutState " + currentText);
                                if(io6InOut.currentIndex==1)
                                    vmusbwave.setIOOutState(6, io6OutState.currentIndex);
                            }
                        }

                        Rectangle  {
                             id: io6InState
                             anchors.verticalCenter: io6InOut.verticalCenter
                             width: 30
                             height: 30
                             color: "green"
                        }
                    }

                    Row {
                        id: io7Ctrl
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        CheckBox {
                            id: io7CtrlEn
                            text: "IO7"

                            onClicked:
                            {
                                if(io7CtrlEn.checked)
                                {
                                    vmusbwave.setIOInOut(7, io7InOut.currentIndex);
                                    if(io7InOut.currentIndex==1)
                                        vmusbwave.setIOOutState(7, io7OutState.currentIndex);
                                }
                                vmusbwave.setIOEnable(7, io7CtrlEn.checked);
                            }
                        }

                        ComboBox {
                            id: io7InOut
                            width:90
                            model:["Input","Output"]

                            onActivated: {
                                console.log("io7InOut " + currentText);
                                vmusbwave.setIOInOut(7, io7InOut.currentIndex);
                            }
                        }

                        ComboBox {
                            id: io7OutState
                            width:60
                            model:["0","1"]

                            onActivated: {
                                console.log("io7OutState " + currentText);
                                if(io7InOut.currentIndex==1)
                                    vmusbwave.setIOOutState(7, io7OutState.currentIndex);
                            }
                        }

                        Rectangle  {
                             id: io7InState
                             anchors.verticalCenter: io7InOut.verticalCenter
                             width: 30
                             height: 30
                             color: "green"
                        }
                    }
                }
            }
        }
    }

    ToolBar {
        id: toolbar
        //anchors.left: vmdsoplot.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        //anchors.margins: 4
        background: Rectangle {
                    color: "#333333"
                }

        Column {
            padding: 4
            spacing: 4
            width: parent.width

            GroupBox{
                width: triggerCtrl.width
                padding: 4
                spacing: 4

                Column {
                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter
                        ToolButton {
                             id: resetDllButton
                             text: "Reset Dll"
                             focusPolicy: Qt.TabFocus
                             //enabled: textArea.selectedText

                             onClicked:{
                                 vmusbwave.resetDll();
                             }
                       }
                        ToolButton {
                             id: resetDevButton
                             text: "Reset Device"
                             focusPolicy: Qt.TabFocus
                             //enabled: textArea.selectedText

                             onClicked:{
                                 vmusbwave.resetDevice();
                             }
                       }
                    }
                }
            }

            GroupBox{
                width: triggerCtrl.width
                padding: 4
                spacing: 4

                Column {
                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: sampleCombox.verticalCenter
                            text: "Sample"
                        }

                        ComboBox {
                            id: sampleCombox

                            onActivated: {
                                console.log("sampleCombox " + currentText);
                                vmusbwave.setsample(Number(currentText));
                            }
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: captureLengthText.verticalCenter
                            text: "Length(KB)"
                        }

                        TextField {
                            id: captureLengthText
                            validator: IntValidator {
                                            id: captureLengthTextIntValidator
                                            bottom: 0;
                                            top: 1000;
                            }

                            onEditingFinished: vmusbwave.setCaptureLength(Number(text));
                        }
                    }

                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter
                        CheckBox {
                            id: captureEn
                            text: "Capture"
                            focusPolicy: Qt.TabFocus
                            //enabled: textArea.selectedText
                            onClicked: vmusbwave.captureEnable(captureEn.checked);
                        }
                    }
                }
            }

            GroupBox {
                padding: 4
                spacing: 4
                width: triggerCtrl.width
                anchors.horizontalCenter: parent.horizontalCenter

                Column {
                    Label{
                        text: "AC/DC"
                    }

                    Column {
                        CheckBox {
                            id: acCh1
                            text: "AC(CH1)"
                            focusPolicy: Qt.TabFocus
                            onClicked: vmusbwave.setAcDcCh1(acCh1.checked);
                        }

                        CheckBox {
                            id: acCh2
                            text: "AC(CH2)"
                            focusPolicy: Qt.TabFocus
                            //enabled: textArea.selectedText
                            onClicked: vmusbwave.setAcDcCh2(acCh2.checked);
                        }
                    }
                }
            }

            GroupBox {
                id: triggerCtrl
                padding: 4
                spacing: 4
                //width: sampleCtrl.width
                anchors.horizontalCenter: parent.horizontalCenter
                title: qsTr("Trigger")

                Column {
                    padding: 4
                    spacing: 4

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: triggerModeCombox.verticalCenter
                            text: "Trigger Mode"
                        }

                        ComboBox {
                            id: triggerModeCombox
                            model:["Auto","Normal"]

                            onActivated: {
                                console.log("triggerModeCombox " + currentText);
                                vmusbwave.setTriggerMode(triggerModeCombox.currentIndex);
                            }
                        }
                    }
                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: triggerStyleCombox.verticalCenter
                            text: "Trigger Style"
                        }

                        ComboBox {
                            id: triggerStyleCombox
                            model:["not trigger","Rising edge","Falling edge","Edge",
                                "Positive Pulse width(>)","Positive Pulse width(>)","Positive Pulse width(<>)",
                                "Negative Pulse width(>)","Negative Pulse width(>)","Negative Pulse width(<>)"]

                            onActivated: {
                                console.log("triggerStyleCombox " + currentText);
                                vmusbwave.setTriggerStyle(triggerStyleCombox.currentIndex);
                            }
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: triggerSourceCombox.verticalCenter
                            text: "Trigger Source"
                        }

                        ComboBox {
                            id: triggerSourceCombox
                            model:["CH1","CH2"]

                            onActivated: {
                                console.log("triggerSourceCombox " + currentText);
                                vmusbwave.setTriggerSource(triggerSourceCombox.currentIndex);
                            }
                        }
                    }

                    Row {
                        width:childrenRect.width
                        height: childrenRect.height
                        spacing: 4

                        Label {
                            anchors.verticalCenter: triggerLevelEdit.verticalCenter
                            text: "Trigger Level(mV)"
                        }

                        TextField {
                            id: triggerLevelEdit
                            validator: IntValidator {
                                            id: triggerLevelEditIntValidator
                                            bottom: -5000;
                                            top: 5000;
                            }

                            onEditingFinished: vmusbwave.setTriggerLevel(Number(text));
                        }
                    }
                }
            }
        }
    }

    ToolBar {
        id: toolbar_osc_xy
        anchors.right: parent.right
        anchors.left: toolbar_dds.right
        anchors.bottom: parent.bottom
        //anchors.bottom: childrenRect.bottom
        anchors.margins: 4

        background: Rectangle {
                    color: "#333333"
                }

        Row {
            spacing: 4

            GroupBox {
                Column  {
                    Label{
                        text: "CH1"
                    }

                    Dial {
                        id: dial_ch1
                        inputMode: Dial.Circular
                        from: 24
                        to: 31
                        stepSize: 1
                        value: 30
                        anchors.horizontalCenter: parent.horizontalCenter

                        property double interal: 0;
                        onMoved: {
                            interal = vmdsoplot.getAxisInterval(value);
                            vmdsoplot.setYAxisRange("CH1", -6*interal, interal);
                            vmdsoplot.redraw();
                        }
                    }
                }
            }

            GroupBox {
                Column  {
                    Label{
                        text: "CH2"
                    }

                    Dial {
                        id: dial_ch2
                        inputMode: Dial.Circular
                        from: 24
                        to: 31
                        stepSize: 1
                        value: 30
                        anchors.horizontalCenter: parent.horizontalCenter

                        property double interal: 0;
                        onMoved: {
                            interal = vmdsoplot.getAxisInterval(value);
                            vmdsoplot.setYAxisRange("CH2", -4*interal, interal);
                            vmdsoplot.redraw();
                        }
                    }
                }
            }

            GroupBox {
                Column  {
                    Label{
                        text: "Time"
                    }

                    Dial {
                        id: dial_time
                        inputMode: Dial.Circular
                        from: 36
                        to: 57
                        stepSize: 1
                        value: 51
                        anchors.horizontalCenter: parent.horizontalCenter

                        property double interal: 0;
                        onMoved: {
                            interal = vmdsoplot.getAxisInterval(value);
                            //console.log("interal " + -4*interal + " " + interal);
                            vmdsoplot.setTimeAxisRange(-5*interal, interal);
                            vmdsoplot.redraw();
                        }
                    }
                }
            }
        }
    }

    VmSimplePlot {
        id: vmdsoplot
        objectName: "objectvmdsoplot"
        anchors.left: toolbar_dds.right
        anchors.top:  parent.top
        anchors.right: toolbar.left
        anchors.bottom:  toolbar_osc_xy.top
        anchors.margins: 4

        property double interal: 0;
        Component.onCompleted: {
            vmdsoplot.addAxis("CH1", "#FFFF00");
            interal = vmdsoplot.getAxisInterval(dial_ch1.value);
            vmdsoplot.setYAxisRange("CH1", -6*interal, interal);

            vmdsoplot.addAxis("CH2", "#FF0000");
            interal = vmdsoplot.getAxisInterval(dial_ch2.value);
            vmdsoplot.setYAxisRange("CH2", -4*interal, interal);
            interal = vmdsoplot.getAxisInterval(dial_time.value);

            vmdsoplot.setTimeAxisRange(-5*interal, interal);

            vmdsoplot.computeAxisPos();
            vmdsoplot.redraw();
        }

        onYAxisRangeChanged: function (trackname, anArray) {
            if(trackname==="CH1")
                vmusbwave.setPlotRangeCh1(anArray);
            else
                vmusbwave.setPlotRangeCh2(anArray);
        }

        onUpdateDataSuccess: vmusbwave.nextCapture();
    }

    VmUsbWave {
        id: vmusbwave
        objectName: "objectvmusbwave"

        onDevStateChanged: function (devstate) {
            vmdsoplot.updatDevState(devstate);

            sampleCombox.model = vmusbwave.getsupportsamples();
            sampleCombox.currentIndex = sampleCombox.indexOfValue(vmusbwave.getsample().toString());
            captureLengthTextIntValidator.top = vmusbwave.getMaxCaptureLength();
            captureLengthText.text = vmusbwave.getCaptureLength().toString();

            acCh1.checked = vmusbwave.getAcDcCh1();
            acCh2.checked = vmusbwave.getAcDcCh2();

            triggerModeCombox.enabled = vmusbwave.isSupportHardTrigger();
            triggerStyleCombox.enabled = vmusbwave.isSupportHardTrigger();
            triggerSourceCombox.enabled = vmusbwave.isSupportHardTrigger();
            triggerLevelEdit.enabled = vmusbwave.isSupportHardTrigger();
            triggerModeCombox.currentIndex = vmusbwave.getTriggerMode();
            triggerStyleCombox.currentIndex = vmusbwave.getTriggerStyle();
            triggerSourceCombox.currentIndex = vmusbwave.getTriggerSource();
            triggerLevelEdit.text = vmusbwave.getTriggerLevel().toString();

            ddsCtrlWave.enabled = vmusbwave.isSupportDds();
            ddsDutyEdit.enabled = vmusbwave.isSupportDds();
            ddsFreqEdit.enabled = vmusbwave.isSupportDds();
            ddsAmplitudeEdit.enabled = vmusbwave.isSupportDds();
            ddsBiasEdit.enabled = vmusbwave.isSupportDds();
            ddsEn.enabled = vmusbwave.isSupportDds();

            if(vmusbwave.isSupportDds())
            {
                ddsDutyEdit.text = "50";
                ddsFreqEdit.text = "1000";
                ddsAmplitudeEdit.text = vmusbwave.getDdsAmplitudeMv().toString();
                ddsBiasEdit.text = vmusbwave.getDdsBiasMv().toString();
                ddsEn.checked = false;
            }


            io0CtrlEn.enabled = vmusbwave.isSupportIo();
            io1CtrlEn.enabled = vmusbwave.isSupportIo();
            io2CtrlEn.enabled = vmusbwave.isSupportIo();
            io3CtrlEn.enabled = vmusbwave.isSupportIo();
            io4CtrlEn.enabled = vmusbwave.isSupportIo();
            io5CtrlEn.enabled = vmusbwave.isSupportIo();
            io6CtrlEn.enabled = vmusbwave.isSupportIo();
            io7CtrlEn.enabled = vmusbwave.isSupportIo();
            if(vmusbwave.isSupportIo())
            {
                io0CtrlEn.checked = false;
                io0InOut.currentIndex = 1;
                io0InState.color = "green";

                io1CtrlEn.checked = false;
                io1InOut.currentIndex = 1;
                io1InState.color = "green";

                io2CtrlEn.checked = false;
                io2InOut.currentIndex = 1;
                io2InState.color = "green";

                io3CtrlEn.checked = false;
                io3InOut.currentIndex = 1;
                io3InState.color = "green";

                io4CtrlEn.checked = false;
                io4InOut.currentIndex = 1;
                io4InState.color = "green";

                io5CtrlEn.checked = false;
                io5InOut.currentIndex = 1;
                io5InState.color = "green";

                io6CtrlEn.checked = false;
                io6InOut.currentIndex = 1;
                io6InState.color = "green";

                io7CtrlEn.checked = false;
                io7InOut.currentIndex = 1;
                io7InState.color = "green";
            }

            console.log("devStateChanged " + devState);
        }

        onUpdateDatas: function (real_length, sample) {
            //for (var i=0; i<anArray.length; i++)
            //    console.log("Array item:", anArray[i])
            vmdsoplot.updateDatas(real_length, sample);
            vmdsoplot.redraw();
        }

        onUpdateIoState: function (state) {
            console.log("onUpdateIoState " + state);
            io0InState.color = ((state&0x0001)==0x0001)? "red" : "green";
            io1InState.color = ((state&0x0002)==0x0002)? "red" : "green";
            io2InState.color = ((state&0x0004)==0x0004)? "red" : "green";
            io3InState.color = ((state&0x0008)==0x0008)? "red" : "green";
            io4InState.color = ((state&0x0010)==0x0010)? "red" : "green";
            io5InState.color = ((state&0x0020)==0x0020)? "red" : "green";
            io6InState.color = ((state&0x0040)==0x0040)? "red" : "green";
            io7InState.color = ((state&0x0080)==0x0080)? "red" : "green";
        }
    }

}

