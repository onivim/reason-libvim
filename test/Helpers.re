open Vim;

let resetBuffer = filePath => {
  input("<esc>");
  input("<esc>");
  command("e!");
  Buffer.openFile(filePath);
};
