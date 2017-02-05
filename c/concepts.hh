#ifndef CONCEPTS_HH_INCLUDED
#define CONCEPTS_HH_INCLUDED


#define HAS_MEMBER_FUNCTION(member)                                                     \
template < typename __Ct, typename __Rt, typename... __Args>                            \
struct has_member_##member{                                                             \
private:                                                                                \
    struct __FALSE_TYPE;                                                                \
    template < typename U > static auto check(int) ->                                   \
                decltype( declval<U>().member(declval<__Args>()...) );                  \
    template < typename U > static auto check(...) -> __FALSE_TYPE ;                    \
                                                                                        \
public:                                                                                 \
    enum { value = std::is_convertible<decltype(check<__Ct>(0)), __Rt>::value };        \
}

#endif // CONCEPTS_HH_INCLUDED
