import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: ddsdlg
    width: 800
    height: 600
    anchors.centerIn: parent
    modal: false
    standardButtons: Dialog.Ok

    property int ddsChannelIndex: 0

    property bool ddsOpen: false
    onOpened: ddsOpen = true;
    onClosed: ddsOpen = false;
    function switchDlg()
    {
        if(ddsOpen) {
            ddsdlg.close()
            ddsOpen = false;
        }
        else {
            ddsdlg.open()
            ddsOpen = true;
        }
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ColumnLayout {
            spacing: 16
            width: parent.width
            enabled: ((ddsChannelIndex==0)&&(vmusbwave.ddsChannelNum>=1)) || ((ddsChannelIndex==1)&&(vmusbwave.ddsChannelNum>=2))

            RowLayout {
                Layout.fillWidth: true

                Item {
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("DDS")+(ddsChannelIndex+1)
                }

                Item {
                    Layout.fillWidth: true
                }
            }

            GridLayout {
                Layout.fillWidth: true
                flow: GridLayout.TopToBottom
                columns: 2
                rowSpacing: 6
                columnSpacing: 6

                RowLayout {
                    Layout.fillWidth: true

                    Item {
                        Layout.fillWidth: true
                    }

                    CheckBox {
                        id: ddsEn
                        text: "Output"
                        focusPolicy: Qt.TabFocus
                        checked: vmusbwave.getDdsOutputEnable(ddsChannelIndex)

                        onClicked: vmusbwave.setDdsOutputEnable(ddsChannelIndex, checked);
                    }

                    Button {
                        id: ddsSyn
                        text: "MultSyn"
                        focusPolicy: Qt.TabFocus
                        onClicked: vmusbwave.setDdsMultSyn();
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }

                RowLayout {
                    id: ddsCtrlMode
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Mode"
                    }

                    ComboBox {
                        id: ddsMode
                        Layout.fillWidth: true
                        model:["Continuous", "Sweep", "Burst"]
                        currentIndex: vmusbwave.getDdsMode(ddsChannelIndex)

                        onActivated: {
                            console.log("ddsMode " + currentText);
                            vmusbwave.setDdsMode(ddsChannelIndex, currentIndex);
                        }
                    }
                }

                RowLayout {
                    id: ddsCtrlWave
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Wave"
                    }

                    ComboBox {
                        id: ddsWave
                        Layout.fillWidth: true
                        model:["Sine", "Square", "Ramp", "Pulse", "Noise", "DC", "Arb"]
                        currentIndex: vmusbwave.getDdsWaveIndex(ddsChannelIndex)

                        onActivated: {
                            console.log("ddsWave " + currentText);
                            vmusbwave.setDdsWaveIndex(ddsChannelIndex, currentIndex);
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsWave.currentIndex == 1 || ddsWave.currentIndex == 2 || ddsWave.currentIndex == 3

                    Label {
                        text: ddsWave.currentIndex == 2? "Symmetry(%)" : "Duty(%)"
                    }

                    TextField {
                        id: ddsDutyEdit
                        validator: DoubleValidator {
                                        id: ddsDutyEditIntValidator
                                        bottom: 5;
                                        top: 95;
                        }
                        text: vmusbwave.getDdsDuty(ddsChannelIndex)

                        onEditingFinished: vmusbwave.setDdsDuty(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Start Phase(deg)"
                    }

                    TextField {
                        id: ddsPhaseEdit
                        Layout.fillWidth: true
                        validator: DoubleValidator {
                                        id: ddsPhaseEditIntValidator
                                        bottom: 0;
                                        top: 360;
                        }
                        text: vmusbwave.getDdsStartPhase(ddsChannelIndex)

                        onEditingFinished: vmusbwave.setDdsStartPhase(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==0 || ddsMode.currentIndex==2

                    Label {
                        text: "Freq(Hz)"
                    }

                    TextField {
                        id: ddsFreqEdit
                        Layout.fillWidth: true
                        validator: DoubleValidator {
                                        id: ddsFreqEditIntValidator
                                        bottom: 1;
                                        top: 2000000;
                        }
                        text: vmusbwave.getDdsFreq(ddsChannelIndex)

                        onEditingFinished: vmusbwave.setDdsFreq(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==1

                    Label {
                        text: "Start Freq(Hz)"
                    }

                    TextField {
                        id: ddsStartFreqEdit
                        Layout.fillWidth: true
                        validator: DoubleValidator {
                                        id: ddsStartFreqEditIntValidator
                                        bottom: 0.1;
                                        top: 5000000.0;
                        }
                        text: vmusbwave.getDdsStartFreq(ddsChannelIndex)

                        onEditingFinished: vmusbwave.setDdsStartFreq(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==1

                    Label {
                        text: "Stop Freq(Hz)"
                    }

                    TextField {
                        id: ddsStopFreqEdit
                        Layout.fillWidth: true
                        validator: DoubleValidator {
                                        id: ddsStopFreqEditIntValidator
                                        bottom: 0.1;
                                        top: 5000000.0;
                        }
                        text: vmusbwave.getDdsStopFreq(ddsChannelIndex)

                        onEditingFinished: vmusbwave.setDdsStopFreq(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==1

                    Label {
                        text: "Sweep Time(us)"
                    }

                    TextField {
                        id: ddsSweepTimeEdit
                        Layout.fillWidth: true
                        validator: IntValidator {
                                        bottom: 1;
                                        top: 100000000;
                        }
                        text: vmusbwave.getDdsSweepTimeUs(ddsChannelIndex).toFixed(0)

                        onEditingFinished: vmusbwave.setDdsSweepTimeUs(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Amplitude(mV)"
                    }

                    TextField {
                        id: ddsAmplitudeEdit
                        Layout.fillWidth: true
                        validator: IntValidator {
                                        id: ddsAmplitudeEditIntValidator
                                        bottom: 10;
                                        top: 10000;
                        }
                        text: vmusbwave.getDdsAmplitudeMv(ddsChannelIndex)

                        onEditingFinished: vmusbwave.setDdsAmplitudeMv(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "Bias(mV)"
                    }

                    TextField {
                        id: ddsBiasEdit
                        Layout.fillWidth: true
                        validator: IntValidator {
                                        id: ddsBiasEditIntValidator
                                        bottom: -2000;
                                        top: 2000;
                        }
                        text: vmusbwave.getDdsBiasMv(ddsChannelIndex)

                        onEditingFinished: vmusbwave.setDdsBiasMv(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==2

                    Label {
                        text: "Burst Style"
                    }

                    ComboBox {
                        id: burstStyle
                        Layout.fillWidth: true
                        model:["N Loops", "Gate"]
                        currentIndex: vmusbwave.getBurstStyle(ddsChannelIndex)

                        onActivated: {
                            console.log("burstStyle " + currentText);
                            vmusbwave.setBurstStyle(ddsChannelIndex, currentIndex);
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==2 && burstStyle.currentIndex==0

                    Label {
                        text: "Cycle Num"
                    }

                    TextField {
                        id: ddsCycleNumEdit
                        Layout.fillWidth: true
                        validator: IntValidator {
                                        bottom: 1;
                                        top: 1000000;
                        }
                        text: vmusbwave.getLoopsNum(ddsChannelIndex)

                        onEditingFinished: vmusbwave.setLoopsNum(ddsChannelIndex, Number(text));
                    }

                    CheckBox {
                        id: ddsCycleNumUnlimited
                        text: "Unlimited"
                        checked: vmusbwave.getLoopsNumInfinity(ddsChannelIndex)

                        onClicked: vmusbwave.setLoopsNumInfinity(ddsChannelIndex, checked);
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==2 && burstStyle.currentIndex==0

                    Label {
                        text: "Burst Period(ns)"
                    }

                    TextField {
                        Layout.fillWidth: true
                        validator: DoubleValidator {
                                        bottom: 1000;
                                        top: 1000000000;
                        }
                        text: vmusbwave.getBurstPeriod(ddsChannelIndex).toFixed(0)

                        onEditingFinished: vmusbwave.setBurstPeriod(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==2 && burstStyle.currentIndex==0

                    Label {
                        text: "Burst Delay(ns)"
                    }

                    TextField {
                        Layout.fillWidth: true
                        validator: DoubleValidator {
                                        bottom: 0;
                                        top: 1000000000;
                        }
                        text: vmusbwave.getBurstDelay(ddsChannelIndex).toFixed(0)

                        onEditingFinished: vmusbwave.setBurstDelay(ddsChannelIndex, Number(text));
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==1 || ddsMode.currentIndex==2

                    Label {
                        text: "Trigger Source"
                    }

                    ComboBox {
                        id: ddsTriggerSourceStyle
                        Layout.fillWidth: true
                        model:["Internal", "External", "Manual"]
                        currentIndex: vmusbwave.getDdsTriggerSource(ddsChannelIndex)

                        onActivated: {
                            console.log("ddsTriggerSourceStyle " + currentText);
                            vmusbwave.setDdsTriggerSource(ddsChannelIndex, currentIndex);
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: ddsMode.currentIndex==1 || ddsMode.currentIndex==2

                    Label {
                        text: "IO"
                    }

                    ComboBox {
                        id: ddsIo
                        Layout.fillWidth: true
                        model: vmusbwave.iOList
                        currentIndex: vmusbwave.getDdsIoIndex(ddsChannelIndex)

                        onActivated: {
                            console.log("ddsIo " + currentText);
                            vmusbwave.setDdsIoIndex(ddsChannelIndex, currentIndex);
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: (ddsMode.currentIndex==1 || ddsMode.currentIndex==2) && (ddsTriggerSourceStyle.currentIndex==0||ddsTriggerSourceStyle.currentIndex==2)

                    Label {
                        text: "Output"
                    }

                    ComboBox {
                        id: ddsOutput
                        Layout.fillWidth: true
                        model:["Close", "Rise Edge", "Fall Edge"]
                        currentIndex: vmusbwave.getDdsOutput(ddsChannelIndex)

                        onActivated: {
                            console.log("ddsOutput " + currentText);
                            vmusbwave.setDdsOutput(ddsChannelIndex, currentIndex);
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    visible: (ddsMode.currentIndex==1 || ddsMode.currentIndex==2) && (ddsTriggerSourceStyle.currentIndex==1)

                    Label {
                        text: "Edge"
                    }

                    ComboBox {
                        id: ddsInputEdge
                        Layout.fillWidth: true
                        model:["Rise Edge", "Fall Edge"]
                        currentIndex: vmusbwave.getDdsInputEdge(ddsChannelIndex)

                        onActivated: {
                            console.log("ddsInputEdge " + currentText);
                            vmusbwave.setDdsInputEdge(ddsChannelIndex, currentIndex);
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    visible: (ddsMode.currentIndex==1 || ddsMode.currentIndex==2) && (ddsTriggerSourceStyle.currentIndex==2)

                    Item {
                        Layout.fillWidth: true
                    }

                    Button {
                        text: "Manual Trigger"
                        focusPolicy: Qt.TabFocus
                        onClicked: vmusbwave.setDdsManualTrigger(ddsChannelIndex);
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}
