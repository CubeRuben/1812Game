#pragma once

#define TEMPLATE_EVENT_DELEGATE(type, name) \
	protected: UPROPERTY(BlueprintAssignable) type On##name##Event; \
	public: FDelegateHandle AddOn##name##Handler(const type::FDelegate& Handler) { return On##name##Event.Add(Handler); }; \
	public: void RemoveOn##name##Handler(const FDelegateHandle& Handle) { On##name##Event.Remove(Handle); };


#define TEMPLATE_GLOBAL_EVENT_DELEGATE(type, name) \
	protected: static type On##name##GlobalEvent; \
	public: static FDelegateHandle AddOn##name##GlobalHandler(const type::FDelegate& Handler) { return On##name##GlobalEvent.Add(Handler); }; \
	public: static void RemoveOn##name##GlobalHandler(const FDelegateHandle& Handle) { On##name##GlobalEvent.Remove(Handle); };