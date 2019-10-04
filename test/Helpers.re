open Vim;

let resetBuffer = filePath => {
  /* Reset clipboard provider */
  Clipboard.setProvider(_ => None);

  input("<esc>");
  input("<esc>");
  command("e!");
  let ret = Buffer.openFile(filePath);

  /* Move cursor to initial position */
  input("g");
  input("g");
  input("0");
  ret;
};
