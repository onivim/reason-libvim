let _ = {
  print_endline("Initializing...");
  Vim.init();
  print_endline("Initializing successful.");

  print_endline("Buffer.test");
  Vim.command("e package.json");
  print_endline("Buffer.test complete");
};
