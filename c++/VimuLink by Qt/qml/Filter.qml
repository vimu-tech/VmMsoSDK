import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1 as Platform

Dialog {
    id: filterdlg
    width: 800
    height: 600
    anchors.centerIn: parent
    modal: false
    standardButtons: Dialog.Ok

    property bool filterOpen: false
    onOpened: filterOpen = true;
    onClosed: filterOpen = false;
    function switchDlg()
    {
        if(filterOpen) {
            filterdlg.close()
            filterOpen = false;
        }
        else {
            filterdlg.open()
            filterOpen = true;
        }
    }

    Action {
        id: filterAxisAction

        property double interal: 0;
        onTriggered:{
            interal = vmdsoplot.getAxisInterval(axisFilterSpin.value);
            vmdsoplot.setYAxisRange("FLT", (-5-axisFilterSlider.value)*interal, interal);
            vmdsoplot.redraw()
        }
    }

    // 文件对话框
    Platform.FileDialog {
        id: fileDialog
        title: "Select fdd File"

        // 多种文件类型过滤
        nameFilters: [
            "Fdd File (*.fdd)"
        ]

        onAccepted: {
            console.log("文件URL:", fileDialog.file)
            // 转换为本地路径
            var filePath = fileDialog.file.toString().replace(/^(file:\/{3})/,"")
            console.log("本地路径:", filePath)
            filterCal.readFddFile(filePath);
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
                        text: "Filter"
                        focusPolicy: Qt.TabFocus
                        checked: filterCal.enable
                        enabled: filterCal.fddLoaded

                        onClicked: {
                            filterCal.enable = checked
                            if(checked)
                            {
                                vmdsoplot.addAxis("FLT", "#55FF22");
                                filterAxisAction.trigger()
                            }
                            else
                                vmdsoplot.removeAxis("FLT")
                            vmdsoplot.redraw()
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        Layout.preferredWidth: 120
                        color: "white"
                        elide: Text.ElideMiddle  // 中间省略号
                        text: filterCal.fddPathFileName
                    }

                    Button {
                        id: fddSelect
                        Layout.fillWidth: true
                        text: "Fdd"

                        onClicked: {
                            fileDialog.open()
                        }
                    }
                }

                SpinBox{
                    id: axisFilterSpin
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
                        filterAxisAction.trigger()
                    }
                }

                Slider {
                    id: axisFilterSlider
                    Layout.fillWidth: true
                    from: -5
                    value: 0
                    to: 5

                    property double interal: 0;
                    onValueChanged: {
                        filterAxisAction.trigger()
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Source"
                    }

                    ComboBox {
                        id: filterSource
                        Layout.fillWidth: true
                        model:["CH1", "CH2", "CH3", "CH4"]
                        currentIndex: fftCal.sourceIndex

                        onActivated: {
                            console.log("filterSource " + currentText);
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
                        id: filterLength
                        Layout.fillWidth: true
                        model: fftCal.lengthList
                        currentIndex: fftCal.lengthIndex

                        onActivated: {
                            console.log("fftLength " + currentText);
                            fftCal.lengthIndex = currentIndex
                        }
                    }
                }
            }
        }
    }
}
