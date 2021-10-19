// 23/7/2019: copied from 2018 Payload format. 
// 6/8/2019 by MM: bugs solved, sign bit was not processed, Vcc en Vbat added to message 0xB
// 5/10/2020 by MM cleanup message 0xA and 0xC removed
// 25/04/2021 adpated for TTN 
// 25/04/2021 added decode Little Endian functions for Hittestress 2021 port 15 en 16

function decodeUplink( input) {

    // decoding functions

    function bytesToFloatLE(bytes) {
        var bits = bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
        var sign = (bits >>> 31 === 0) ? 1.0 : -1.0;
        var exponent = bits >>> 23 & 0xff;
        var mantisse = (exponent === 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;
        var floating_point = sign * mantisse * Math.pow(2, exponent - 127) / 0x800000;
        return floating_point;
    }

    function bytesToInt16LE(bytes) {
        var val = bytes[1] << 8 | bytes[0];
        return (val < 0x8000) ? val : val - 0x10000;
    }

    function bytesToInt16(bytes) {
        var val = bytes[0] << 8 | bytes[1];
        return (val < 0x8000) ? val : val - 0x10000;
    }

    function bytesToInt32LE(bytes) {
        var val = bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
        return (val < 0x80000000) ? val : val - 0x10000000;
    }


    function bytesToInt32(bytes) {
        var val = bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
        return (val < 0x80000000) ? val : val - 0x10000000;
    }
    // end of decoding functions


    // start of payload formatter 
    var decoded = {};
    var port = input.fPort;
    var bytes = input.bytes;

    // decode Hittestress 2019
    // decode location message
    if (port == 11) {

        decoded.latitude = bytesToInt32(bytes.slice(0)) / 10000000;
        decoded.longitude = bytesToInt32(bytes.slice(4)) / 10000000;
        decoded.alt = bytesToInt16(bytes.slice(8));
        decoded.hdop = bytesToInt16(bytes.slice(10)) / 1000;
        decoded.SwVer = bytes.slice(12)[0];
    }

    // decode status message
    else if (port == 13) {
        decoded.CpuTemp = bytesToInt16(bytes.slice(0)) / 10;
        decoded.Vbat = bytesToInt16(bytes.slice(2)) / 100;
        decoded.LowBat = bytes.slice(4) & 0x01;
    }

    // decode data message
    else if (port == 14) {
        decoded.pm2p5 = bytesToInt16(bytes.slice(0)) / 10;
        decoded.pm10 = bytesToInt16(bytes.slice(2)) / 10;
        decoded.rh = bytesToInt16(bytes.slice(4)) / 10;
        decoded.temp = bytesToInt16(bytes.slice(6)) / 10;
    }

    // decode Hittestress 2021
    // decode data
    else if (port == 15) {
        decoded.temp = bytesToInt16LE(bytes.slice(0)) / 100.0;
        decoded.rh = bytesToInt16LE(bytes.slice(2)) / 100.0;
        decoded.pm2p5 = bytesToInt16LE(bytes.slice(4)) / 100.0;
        decoded.pm10 = bytesToInt16LE(bytes.slice(6)) / 100.0;
        if( bytes.length >= 10) 
          decoded.vbat = bytesToInt16LE(bytes.slice(8)) / 1000.0;
    }
    //  decode status
    else if (port == 16) {
        decoded.latitude = bytesToFloatLE(bytes.slice(0));
        decoded.longitude = bytesToFloatLE(bytes.slice(4));
        decoded.alt = bytesToInt16LE(bytes.slice(8));
        decoded.hdop = bytesToInt16LE(bytes.slice(10)) / 1000.0;
        decoded.vbat = bytesToInt16LE(bytes.slice(12)) / 1000.0;
        decoded.SwVer = bytesToInt16LE(bytes.slice(14)) / 100.0;
    }

    return { data: decoded, warnings:[], errors:[] };
}
