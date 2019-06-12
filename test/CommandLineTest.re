open TestFramework;
open Vim;

let reset = () => Helpers.resetBuffer("test/testfile.txt");

describe("CommandLine", ({describe, _}) =>  {
  describe("getText", ({test, _}) => {
    test("simple command line", ({expect}) => {
        let _ = reset();

        input(":");
        input("a");

        switch (CommandLine.getText()) {
        | Some(v) => expect.string(v).toEqual("a");
        | None => expect.int(0).toBe(1);
        };

        input("b");

        switch (CommandLine.getText()) {
        | Some(v) => expect.string(v).toEqual("ab");
        | None => expect.int(0).toBe(1);
        };

        input("c");

        switch (CommandLine.getText()) {
        | Some(v) => expect.string(v).toEqual("abc");
        | None => expect.int(0).toBe(1);
        };
    });
  });

  describe("getPosition", ({test, _}) => {
    test("simple command line", ({expect}) => {
        let _ = reset();
        input(":");
        expect.int(CommandLine.getPosition()).toBe(0);

        input("a");
        expect.int(CommandLine.getPosition()).toBe(1);

        input("b");
        expect.int(CommandLine.getPosition()).toBe(2);

        input("c");
        expect.int(CommandLine.getPosition()).toBe(3);
    });
  });
});
