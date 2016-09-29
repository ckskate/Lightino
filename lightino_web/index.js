"use strict";

var baseURL = "http://cnnr.local/arduino/set/";

/** Utility function to format the number correctly for the api. */
function leftPad(val) {
  if (val < 100) {
    return "0" + val;
  }
  return val;
}

/** Converts a 0-255 number to a 128-255 number. */
function clipToRange(val) {
  return Math.round((val / 255) * 127) + 128;
}

$(document).mousemove(function(e) {
  e.preventDefault();
  var screenWidth = $(this).width();
  var screenHeight = $(this).height();
  var x = Math.round((e.clientX / screenWidth) * 255);
  var y = Math.round((e.clientY / screenHeight) * 40) + 30;
  $(document.body).css("backgroundColor", "hsl(" + x + ",100%," + y + "%)");
});

$(document).click(function(e) {
  e.preventDefault();
  var color = $(document.body).css("backgroundColor");
  var colors = color.split(",");
  var r = colors[0].substring(4);
  var g = colors[1];
  var b = (colors[2].length > 1) ? colors[2].substring(0, colors[2].length - 1) : colors[2];

  // Ajax call to the light strip api on the Arduino Yun.
  $.ajax({
    url: baseURL + leftPad(clipToRange(Number(r))) + "/"
      + leftPad(clipToRange(Number(g))) + "/" + leftPad(clipToRange(Number(b))) + "/"
  });
});
