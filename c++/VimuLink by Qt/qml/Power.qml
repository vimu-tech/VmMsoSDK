import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: powerdlg
    width: 800
    height: 600
    anchors.centerIn: parent
    modal: false
    standardButtons: Dialog.Ok

    property var usbModel: vmusbwave

    Connections {
        target: powerdlg.usbModel
        //function onSampleIndexChanged() {
        //    console.log("powerdlg sampleIndexChanged " + powerdlg.usbModel.getSample());
        //}
    }

    property bool powerOpen: false
    onOpened: powerOpen = true;
    onClosed: powerOpen = false;
    function switchDlg()
    {
        if(powerOpen) {
            powerdlg.close()
            powerOpen = false;
        }
        else {
            powerdlg.open()
            powerOpen = true;
        }
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        RowLayout {
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
                        id: posPowerEn
                        enabled: usbModel.powerSupport
                        text: "0~12V"
                        focusPolicy: Qt.TabFocus

                        onClicked: {
                            usbModel.setPosPowerEn(checked);
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }

                Label {
                    Layout.alignment: Qt.AlignCenter
                    text: posPowerSlider.value.toFixed(2)+"V"
                }

                Slider {
                    id: posPowerSlider
                    Layout.fillWidth: true
                    from: 0
                    value: 1
                    to: 12

                    property double interal: 0;
                    onValueChanged: {
                        usbModel.setPosPowerV(value);
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                RowLayout {
                    Layout.fillWidth: true

                    Item {
                        Layout.fillWidth: true
                    }

                    CheckBox {
                        id: negPowerEn
                        enabled: usbModel.powerSupport
                        text: "-12V~0V"
                        focusPolicy: Qt.TabFocus

                        onClicked: {
                            onClicked: {
                                usbModel.setNegPowerEn(checked);
                            }
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }

                Label {
                    Layout.alignment: Qt.AlignCenter
                    text: negPowerSlider.value.toFixed(2)+"V"
                }

                Slider {
                    id: negPowerSlider
                    Layout.fillWidth: true
                    from: -12
                    value: -1
                    to: 0

                    property double interal: 0;
                    onValueChanged: {
                        usbModel.setNegPowerV(value);
                    }
                }
            }
        }
    }
}
