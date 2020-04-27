type context = {revEffects: ref(list(Effect.t))};

module Internal = {
  let unhandledEffectEvent = Event.create();
  let currentContext: ref(option(context)) = ref(None);
};

let onUnhandledEffect = handler => {
  Event.add(handler, Internal.unhandledEffectEvent);
};

let notifyEffect = effect => {
  switch (Internal.currentContext^) {
  | None => Event.dispatch(effect, Internal.unhandledEffectEvent)
  | Some(context) => context.revEffects := [effect, ...context.revEffects^]
  };
};

let runAndTrackEffects = f => {
  let context =
    switch (Internal.currentContext^) {
    | None => {revEffects: ref([])}
    | Some(context) => context
    };

  Internal.currentContext := Some(context);
  let ret = f();
  let effects =
    switch (Internal.currentContext^) {
    | None => []
    | Some(eff) => eff.revEffects^ |> List.rev
    };
  Internal.currentContext := None;
  (ret, effects);
};
