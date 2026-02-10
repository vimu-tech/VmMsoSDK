import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: fftdlg
    width: 800
    height: 600
    anchors.centerIn: parent
    modal: false
    standardButtons: Dialog.Ok

    property var usbModel: vmusbwave

    Connections {
        target: fftdlg.usbModel
        function onSampleIndexChanged() {
            console.log("fftdlg sampleIndexChanged " + fftdlg.usbModel.getSample());
            vmdsoplot.setMathTimeAxisRange(0, fftdlg.usbModel.getSample()/2)
        }
    }

    property bool fftOpen: false
    onOpened: fftOpen = true;
    onClosed: fftOpen = false;
    function switchDlg()
    {
        if(fftOpen) {
            fftdlg.close()
            fftOpen = false;
        }
        else {
            fftdlg.open()
            fftOpen = true;
        }
    }

    Action {
        id: fftAxisAction

        property double interal: 0;
        onTriggered:{
            interal = vmdsoplot.getAxisInterval(axisFFTSpin.value);
            vmdsoplot.setMathYAxisRange("FFT", (-5-axisFFTSlider.value)*interal, interal);
        }
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ColumnLayout {
            spacing: 16
            width: parent.width

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                RowLayout {
                    Layout.fillWidth: true

                    Item {
                        Layout.fillWidth: true
                    }

                    CheckBox {
                        id: fftEn
                        text: "FFT"
                        focusPolicy: Qt.TabFocus
                        checked: fftCal.fftCheck

                        onClicked: {
                            if(checked)
                            {
                                vmdsoplot.addMathAxis("FFT", "#FF5522")
                                vmdsoplot.setMathTimeAxisRange(0, fftdlg.usbModel.getSample()/2)
                            }
                            else
                                vmdsoplot.removeMathAxis("FFT");
                            fftAxisAction.trigger()
                            vmdsoplot.redraw();
                            fftCal.fftCheck = checked
                        }
                    }
                }

                SpinBox{
                    id: axisFFTSpin
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
                        fftAxisAction.trigger()
                        vmdsoplot.redraw();
                    }
                }

                Slider {
                    id: axisFFTSlider
                    Layout.fillWidth: true
                    from: -5
                    value: -4
                    to: 5

                    property double interal: 0;
                    onValueChanged: {
                        fftAxisAction.trigger()
                        vmdsoplot.redraw();
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Source"
                    }

                    ComboBox {
                        id: fftSource
                        Layout.fillWidth: true
                        model:["CH1", "CH2", "CH3", "CH4"]
                        currentIndex: fftCal.sourceIndex

                        onActivated: {
                            console.log("fftSource " + currentText);
                            fftCal.sourceIndex = currentIndex
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Length"
                    }

                    ComboBox {
                        id: fftLength
                        Layout.fillWidth: true
                        model: fftCal.lengthList
                        currentIndex: fftCal.lengthIndex

                        onActivated: {
                            console.log("fftLength " + currentText);
                            fftCal.lengthIndex = currentIndex
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Window"
                    }

                    ComboBox {
                        id: fftWindow
                        Layout.fillWidth: true
                        model: fftCal.windowStyleList
                        currentIndex: fftCal.windowStyleIndex

                        onActivated: {
                            console.log("fftWindow " + currentText);
                            fftCal.windowStyleIndex = currentIndex
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Window Para"
                    }

                    TextField {
                        id: fftWindowParaEdit
                        validator: DoubleValidator {
                                        id: ddsDutyEditIntValidator
                                        bottom: -10000;
                                        top: 10000;
                        }
                        text: fftCal.windowPara

                        onEditingFinished: fftCal.windowPara = Number(text)
                    }
                }

            }
        }
    }
}
