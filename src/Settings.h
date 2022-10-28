#pragma once

namespace Settings
{
	template <class T>
	class Setting
	{
	public:
		using value_type = T;

		Setting(
			std::string_view a_group,
			std::string_view a_key,
			value_type a_default) noexcept :
			_group(a_group),
			_key(a_key),
			_value(a_default)
		{}

		[[nodiscard]] auto group() const noexcept -> std::string_view { return this->_group; }
		[[nodiscard]] auto key() const noexcept -> std::string_view { return this->_key; }

		template <class Self>
		[[nodiscard]] auto&& get(this Self&& a_self) noexcept
		{
			return std::forward<Self>(a_self)._value;
		}

		template <class Self>
		[[nodiscard]] auto&& operator*(this Self&& a_self) noexcept
		{
			return std::forward<Self>(a_self).get();
		}

	private:
		std::string_view _group;
		std::string_view _key;
		value_type _value;
	};

	using bSetting = Setting<bool>;
	using iSetting = Setting<std::int64_t>;
	using sSetting = Setting<std::string>;

#define MAKE_SETTING(a_type, a_group, a_key, a_default) \
	inline auto a_key = a_type(a_group##sv, #a_key##sv, a_default)

	MAKE_SETTING(bSetting, "Fixes", ActorIsHostileToActor, true);

#undef MAKE_SETTING

	inline std::vector<
		std::variant<
			std::reference_wrapper<bSetting>,
			std::reference_wrapper<iSetting>,
			std::reference_wrapper<sSetting>>>
		settings;

	inline void load()
	{
		const auto config = toml::parse_file("Data/F4SE/Plugins/VRFPSStabilizerFallout.toml"sv);

#define LOAD(a_setting)                                                              \
	settings.push_back(std::ref(a_setting));                                         \
	if (const auto tweak = config[a_setting.group()][a_setting.key()]; tweak) {      \
		if (const auto value = tweak.as<decltype(a_setting)::value_type>(); value) { \
			*a_setting = value->get();                                               \
		} else {                                                                     \
			throw std::runtime_error(                                                \
				fmt::format(                                                         \
					"setting '{}.{}' is not of the correct type: expected '{}'"sv,   \
					a_setting.group(),                                               \
					a_setting.key(),                                                 \
					typeid(decltype(a_setting)::value_type).name()));                \
		}                                                                            \
	}

		LOAD(ActorIsHostileToActor);

		std::sort(
			settings.begin(),
			settings.end(),
			[](auto&& a_lhs, auto&& a_rhs) {
				const auto get = [](auto&& a_val) {
					return std::make_pair(
						a_val.get().group(),
						a_val.get().key());
				};

				const auto [lgroup, lkey] = std::visit(get, a_lhs);
				const auto [rgroup, rkey] = std::visit(get, a_rhs);
				if (lgroup != rgroup) {
					return lgroup < rgroup;
				} else {
					return lkey < rkey;
				}
			});
	}
}
