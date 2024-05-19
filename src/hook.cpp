#include "hook.h"



class OurEventSink : public RE::BSTEventSink<RE::TESCombatEvent> 
{
    OurEventSink() = default;
    OurEventSink(const OurEventSink&) = delete;
    OurEventSink(OurEventSink&&) = delete;
    OurEventSink& operator=(const OurEventSink&) = delete;
    OurEventSink& operator=(OurEventSink&&) = delete;

public:
    static OurEventSink* GetSingleton() {
        static OurEventSink singleton;
        return &singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(const RE::TESCombatEvent* event, RE::BSTEventSource<RE::TESCombatEvent>*) {
        auto targetName = event->targetActor;
        auto sourceName = event->actor;
        auto Dragon = sourceName->As<RE::Actor>();
        const auto ActorTypeDragon = RE::BGSKeyword::LookupByEditorID("ActorTypeDragon")->As<RE::BGSKeyword>();
        auto UDDragonAISpell = RE::TESForm::LookupByEditorID<RE::SpellItem>("aaaUDDragonAISpell");

        bool IsActorTypeDragon = Dragon->HasKeyword(ActorTypeDragon);
        bool HasUDDragonAISpell = Dragon->HasSpell(UDDragonAISpell);

        if (!IsActorTypeDragon) {
            RE::BSEventNotifyControl::kContinue;
        }
        if (!HasUDDragonAISpell) {
            RE::BSEventNotifyControl::kContinue;
        }
        
        auto UDQuest = RE::TESForm::LookupByEditorID<RE::TESQuest>("aaaUDInitQuest");

        if (UDQuest) {

            if (event->newState == RE::ACTOR_COMBAT_STATE::kCombat && !Dragon->IsBeingRidden())
            {
                Dragon->AddSpell(UDDragonAISpell);
            }
        }
        RE::BSEventNotifyControl::kContinue;

    }
};

void IHooks::install() {

    auto eventSink = OurEventSink::GetSingleton();

    // ScriptSource
    auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
    eventSourceHolder->AddEventSink<RE::TESCombatEvent>(eventSink);

}













































// auto vmhandle = RE::SkyrimVM::GetSingleton()->handlePolicy.GetHandleForObject(77, UDQuest);

// UDQuest
// RE::BSScript::IObjectHandlePolicy::GetHandleForObject(77, UDQuest);
// RE::VMHandle vmhandle;
// return RE::BSEventNotifyControl::kContinue;
// RE::VMHandle* vmhandle = RE::BSScript::IObjectHandlePolicy::GetHandleForObject(77, UDQuest);
// event->newState

// std::string* a = "UD_OnCombat"
// static inline const char* s = "UD_OnCombat";
// RE::BSFixedString eventName = s;
// event->newState == RE::ACTOR_COMBAT_STATE::kCombat &&  Dragon->IsBeingRidden() ? Dragon->AddSpell(UDDragonAISpell)
// RE::BSScript::Internal::VirtualMachine::GetSingleton()->DispatchMethodCall2(vmhandle, nullptr, eventName,(Dragon),)
// RE::BSScript::IFunctionArguments::operator()<Dragon, event->newState>
// RE::SkyrimVM::GetSingleton()->RelayEvent(vmhandle, &eventName, <Dragon>,)

// auto targetName = event->target->GetBaseObject()->GetName();
// auto sourceName = event->cause->GetBaseObject()->GetName();

// UInt32 UDInitQuestFormID;
// UInt32 UDSpellFormID;
// UInt32 ActorTypeDragon;

// class CombatEventHandler : public BSTEventSink<TESCombatEvent>
// {
// public:
// 	EventResult ReceiveEvent(TESCombatEvent* evn, EventDispatcher<TESCombatEvent>* dispatcher) override
// 	{
// 		const auto target = DYNAMIC_CAST(evn->target, TESObjectREFR, Actor);
// 		const auto source = DYNAMIC_CAST(evn->source, TESObjectREFR, Actor);

// 		D(
// 			const char* casterName = UPCommon::GetActorName(source);
// 			const char* targetName = UPCommon::GetActorName(target);

// 			switch (evn->state)
// 			{
// 				case 0:
// 				_DMESSAGE("[DEBUG] Not in combat:\n  caster: %s", casterName);
// 				break;
// 				case 1:
// 				_DMESSAGE("[DEBUG] In combat:\n  caster: %s\n  target: %s", casterName, targetName);
// 				break;
// 				case 2:
// 				_DMESSAGE("[DEBUG] Searching:\n  caster: %s\n  target: %s", casterName, targetName);
// 				break;
// 			}
// 		);

// 		if (!HasDragonKeyword(source))
// 			return kEvent_Continue;

// 		if (HasUDDragonAISpell(source))
// 		{
// 			D(_DMESSAGE("[DEBUG] %s has spell already", casterName));
// 			return kEvent_Continue;
// 		}

// 		D(_DMESSAGE("[DEBUG] Combat event with dragon as source, dispatch Event UD_OnCombat"));

// 		const auto form = LookupFormByID(UDInitQuestFormID);

// 		if (form)
// 		{
// 			const auto quest = DYNAMIC_CAST(form, TESForm, TESQuest);

// 			T(_DMESSAGE("[TRACE} quest ptr 0x%016" PRIXPTR, quest));
// 			if (quest)
// 			{
// 				const UInt64 vmhandle = UPCommon::GetVMHandleForQuest(quest);
// 				if (vmhandle)
// 				{
// 					static BSFixedString eventName("UD_OnCombat");
// 					UPCommon::EventFunctor2<Actor*, UInt32>(eventName, source, evn->state)(vmhandle);
// 				}
// 			}
// 		}

// 		return kEvent_Continue;
// 	}
// };
