object "A" {
  code {
    sstore(dataoffset(".metadata.x"), 0)
  }

  object ".metadata" {
    code {}
    data "x" "ABC"
  }
}
// ----
// Assembly:
//     /* "source":26:66   */
//   sstore(data_f649fddfac28d2c346e4fe117e4a377b531b82bfd6175516f31da92e9ef22bcf, data_27fd9bb70fcea8125f7e0ba4eaeda4fb5ff28f5535e452dc1b70a42f567f4d75)
//     /* "source":71:121   */
//   0x03
//     /* "source":78:79   */
//   0x00
//     /* "source":71:121   */
//   sstore
// stop
// data_27fd9bb70fcea8125f7e0ba4eaeda4fb5ff28f5535e452dc1b70a42f567f4d75 58595a
// data_7c36b76a3a2d2b05e0ba960b771867cfb7e58a7013ae1146e66d5880dd8a4a41 545253
// data_f649fddfac28d2c346e4fe117e4a377b531b82bfd6175516f31da92e9ef22bcf 555657
//
// auxdata: 0x414243
// Bytecode: 600b600e556003600055fe58595a555657414243
// Opcodes: PUSH1 0xB PUSH1 0xE SSTORE PUSH1 0x3 PUSH1 0x0 SSTORE INVALID PC MSIZE GAS SSTORE JUMP JUMPI COINBASE TIMESTAMP NUMBER
// SourceMappings: 26:40:0:-:0;;;71:50;78:1;71:50
