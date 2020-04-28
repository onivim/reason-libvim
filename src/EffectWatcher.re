type effectStack = list(Effect.t);

module Internal = {
  let unhandledEffectEvent = Event.create();
  let currentEffectStack: ref(option(effectStack)) = ref(None);
};

let onUnhandledEffect = handler => {
  Event.add(handler, Internal.unhandledEffectEvent);
};

let notifyEffect = effect => {
  switch (Internal.currentEffectStack^) {
  | None => Event.dispatch(effect, Internal.unhandledEffectEvent)
  | Some(effects) => 
    Internal.currentEffectStack := Some([effect, ...effects])
  };
};

let runAndTrackEffects = f => {
  let effectStack =
    switch (Internal.currentEffectStack^) {
    | None => []
    | Some(effectStack) => effectStack
    };

  Internal.currentEffectStack := Some(effectStack);

  // Run side-effectful operation - it's expected that the operation [f]
  // will call [notifyEffect] to let us know that some side-effect occurred
  // over the course of running the command.
  let ret = f();

  let effects =
    switch (Internal.currentEffectStack^) {
    | None => []
    | Some(eff) => eff |> List.rev
    };
  Internal.currentEffectStack := None;
  (ret, effects);
};
