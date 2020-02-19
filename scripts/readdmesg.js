#!/usr/bin/env node
"use strict";

let logaddr = 0x200005c4
const maxLog = 2048
const wiggleRoom = 512

let fs = require("fs")
let child_process = require("child_process")

function fatal(msg) {
    console.log("Fatal error:", msg)
    process.exit(1)
}

function looksLikeLog(buf, pos) {
   let len = buf.readUInt32LE(pos)
   if (5 < len && len < maxLog && buf[pos + 4 + len] === 0) {
        for (let i = 0; i < len; ++i)
          if (buf[pos + 4 + i] == 0)
              return false
        return true
   }
   return false
}

function main() {
    let addr = logaddr - wiggleRoom
    let logSize = maxLog + wiggleRoom * 2
    let cmd = `telnet_port disabled; init; halt; `
    cmd += `set M(0) 0; mem2array M 8 ${addr} ${logSize}; resume; parray M; shutdown`

    let args = [
        "-f", "interface/cmsis-dap.cfg",
        "-f", "target/stm32f4x.cfg",
        "-c", cmd]


       let openocdBin = "openocd"

    //console.log("Starting " + openocdBin + " " + args.join(" "))

        child_process.execFile(openocdBin, args, {
            maxBuffer: 1 * 1024 * 1024,
        }, (err, stdout, stderr) => {
            if (err) {
                fatal("error: " + err.message)
            }
            let buf = Buffer.alloc(logSize)
            for (let l of stdout.split(/\r?\n/)) {
                let m = /^M\((\d+)\)\s*=\s*(\d+)/.exec(l)
                if (m) {
                    buf[parseInt(m[1])] = parseInt(m[2])
                }
            }

            for (let i = 0; i < wiggleRoom * 2; i += 4) {
              if (looksLikeLog(buf, i)) {
                let len = buf.readUInt32LE(i)
                console.log(`\n*** Logs at ${(addr + i).toString(16)}, ${len} bytes\n`)
                console.log(buf.slice(i + 4, i + 4 + len).toString("binary"))
              }
            }
        })
}

main()
