import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtGraphicalEffects 1.12 as GE

Item {
	id: svg2qml
	property var scaleShape: Qt.size(1, 1)
	property bool thinkLines: false
	Item {
		width: svg2qml.width
		height: svg2qml.height
		Item {
			id: svg2731
			width: svg2qml.width
			height: svg2qml.height
			Item {
				id: layer1
				width: svg2qml.width
				height: svg2qml.height
				Shape {
					id: path5300
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 75.7131,2.35608 C 36.0195,2.35608 3.84153,34.1589 3.84153,73.3898 C 3.84153,112.621 36.0195,144.423 75.7131,144.423 C 115.407,144.423 147.585,112.621 147.585,73.3898 C 147.585,34.1589 115.407,2.35608 75.7131,2.35608 Z"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: (svg2qml.thinkLines)? "transparent" : "#ff000000"
					}
					opacity: 0.2
				}
				Shape {
					id: path5302
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 75.7131,131.888 C 108.38,131.888 134.901,105.676 134.901,73.3897 C 134.901,41.1036 108.38,14.8914 75.7131,14.8914 C 43.0462,14.8914 16.5247,41.1036 16.5247,73.3897 C 16.5247,105.676 43.0462,131.888 75.7131,131.888"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: (svg2qml.thinkLines)? "transparent" : "#ff000000"
					}
					opacity: 0.443
				}
				Shape {
					id: path5304
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 75.7131,19.0699 C 45.3592,19.0699 20.7525,43.3897 20.7525,73.3898 C 20.7525,103.39 45.3592,127.71 75.7131,127.71 C 106.067,127.71 130.674,103.39 130.674,73.3898 C 130.674,43.3897 106.067,19.0699 75.7131,19.0699 Z"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: (svg2qml.thinkLines)? "transparent" : "#ff494949"
					}
				}
				Shape {
					id: path5306
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 75.7131,19.0699 C 45.3592,19.0699 20.7525,43.3897 20.7525,73.3898 C 20.7525,103.39 45.3592,127.71 75.7131,127.71 C 106.067,127.71 130.674,103.39 130.674,73.3898 C 130.674,43.3897 106.067,19.0699 75.7131,19.0699 Z"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: "#00333333"
					}
					Item {
						width: svg2qml.width
						height: svg2qml.height
						Shape {
							width: svg2qml.width
							height: svg2qml.height
							ShapePath {
								PathSvg {
									path: "M 75.7131,19.0699 C 45.3592,19.0699 20.7525,43.3897 20.7525,73.3898 C 20.7525,103.39 45.3592,127.71 75.7131,127.71 C 106.067,127.71 130.674,103.39 130.674,73.3898 C 130.674,43.3897 106.067,19.0699 75.7131,19.0699 Z"
								}
								strokeWidth: 0
								strokeColor: "transparent"
								fillGradient: LinearGradient {
									x1:52.5; y1:309.115
									x2:52.5; y2:298.351
									GradientStop { position: 0; color: (svg2qml.thinkLines)? "transparent" : "#ff333333"; } 
									GradientStop { position: 1; color: (svg2qml.thinkLines)? "transparent" : "#00333333"; } 
								}
							}
							transform: Matrix4x4 {matrix: Qt.matrix4x4(0, -4.22774, 0, 1392.07, -4.17845, 0, 0, 240.528, 0, 0, 1, 0, 0, 0, 0, 1); }
						}
						layer.enabled: true
						layer.effect: GE.OpacityMask {
							maskSource: Shape {
								width: svg2qml.width
								height: svg2qml.height
								ShapePath {
									PathSvg {
										path: "M 75.7131,19.0699 C 45.3592,19.0699 20.7525,43.3897 20.7525,73.3898 C 20.7525,103.39 45.3592,127.71 75.7131,127.71 C 106.067,127.71 130.674,103.39 130.674,73.3898 C 130.674,43.3897 106.067,19.0699 75.7131,19.0699 Z"
									}
									fillColor: "#000000"
									strokeWidth: 0
									strokeColor: "transparent"
								}
							}
						}
					}
				}
				Shape {
					id: path5308
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 75.7131,23.2483 C 47.6941,23.2483 24.9802,45.6974 24.9802,73.3898 C 24.9802,101.082 47.6941,123.531 75.7131,123.531 C 103.732,123.531 126.446,101.082 126.446,73.3898 C 126.446,45.6974 103.732,23.2483 75.7131,23.2483 Z"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: (svg2qml.thinkLines)? "transparent" : "#ff2e2e2e"
					}
				}
				Shape {
					id: path5310
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 75.7131,23.2483 C 47.6941,23.2483 24.9802,45.6974 24.9802,73.3898 C 24.9802,101.082 47.6941,123.531 75.7131,123.531 C 103.732,123.531 126.446,101.082 126.446,73.3898 C 126.446,45.6974 103.732,23.2483 75.7131,23.2483 Z"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: "#00595959"
					}
					Item {
						width: svg2qml.width
						height: svg2qml.height
						Shape {
							width: svg2qml.width
							height: svg2qml.height
							ShapePath {
								PathSvg {
									path: "M 75.7131,23.2483 C 47.6941,23.2483 24.9802,45.6974 24.9802,73.3898 C 24.9802,101.082 47.6941,123.531 75.7131,123.531 C 103.732,123.531 126.446,101.082 126.446,73.3898 C 126.446,45.6974 103.732,23.2483 75.7131,23.2483 Z"
								}
								strokeWidth: 0
								strokeColor: "transparent"
								fillGradient:  RadialGradient {
									centerX: 40.3062; centerY: 342.4;
									focalX: 40.3062; focalY: 342.4;
									centerRadius: 12
									focalRadius: 0
									GradientStop { position: 0; color: (svg2qml.thinkLines)? "transparent" : "#ff595959"; } 
									GradientStop { position: 1; color: (svg2qml.thinkLines)? "transparent" : "#00595959"; } 
								}
							}
							transform: Matrix4x4 {matrix: Qt.matrix4x4(-0.177749, -9.16879, 0, 3300.02, -6.21518, 0.256142, 0, 233.801, 0, 0, 1, 0, 0, 0, 0, 1); }
						}
						layer.enabled: true
						layer.effect: GE.OpacityMask {
							maskSource: Shape {
								width: svg2qml.width
								height: svg2qml.height
								ShapePath {
									PathSvg {
										path: "M 75.7131,23.2483 C 47.6941,23.2483 24.9802,45.6974 24.9802,73.3898 C 24.9802,101.082 47.6941,123.531 75.7131,123.531 C 103.732,123.531 126.446,101.082 126.446,73.3898 C 126.446,45.6974 103.732,23.2483 75.7131,23.2483 Z"
									}
									fillColor: "#000000"
									strokeWidth: 0
									strokeColor: "transparent"
								}
							}
						}
					}
					opacity: 0.6625
				}
				Shape {
					id: path5312
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 79.9409,48.3191 L 46.1189,81.7467 L 58.8022,94.282 L 79.9409,73.3898 L 101.08,94.282 L 113.763,81.7467 L 79.9409,48.3191 Z"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: (svg2qml.thinkLines)? "transparent" : "#ff000000"
					}
					opacity: 0.38
				}
				Shape {
					id: path5314
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 75.7131,48.3191 L 41.8912,81.7467 L 54.5744,94.282 L 75.7131,73.3898 L 96.8518,94.282 L 109.535,81.7467 L 75.7131,48.3191 Z"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: (svg2qml.thinkLines)? "transparent" : "#ff79ff00"
					}
				}
				Shape {
					id: path5316
					width: svg2qml.width
					height: svg2qml.height
					ShapePath {
						PathSvg {
							path: "M 75.7131,48.3191 L 41.8912,81.7467 L 54.5744,94.282 L 75.7131,73.3898 L 75.7131,48.3191 Z"
						}
						strokeColor: (svg2qml.thinkLines)? "black"  : "transparent"
						strokeWidth: (svg2qml.thinkLines)? 1 : 7
						fillColor: (svg2qml.thinkLines)? "transparent" : "#ff57b700"
					}
				}
			}
		}
	}
}
