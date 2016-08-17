#ifndef BPUTIL_FACTORY_HPP
#define BPUTIL_FACTORY_HPP

#include <unordered_map>
#include <memory>

namespace bp {

    template <typename Derived, typename Base>
    using enable_if_instance<Derived, Base> = std::enable_if<std::is_base_of<Base, Derived>::value>;

    template <typename Base>
    class abstract_creator {
    public:
        abstract_creator() {}
        virtual ~abstract_creator(){}
        virtual std::shared_ptr<Base> create() const = 0;
    };


    template <typename T, typename Base, typename = enable_if_instance<T, Base>::type>
    class concrete_creator : public abstract_creator<Base> {
    public:
        concrete_creator() { }
        virtual ~concrete_creator() {}
        virtual std::shared_ptr<Base> create() const override {
            return std::static_pointer_cast<Base>(std::make_shared<T>());
        }
    };

    template <typename Base, typename IdType=std::string>
    class factory {
        using factory_map = std::unordered_map<IdType, std::shared_ptr<abstract_creator<Base>>>;
    public:

        virtual ~factory(){};

        template <typename T>
        void add(const IdType &_name) {
            register_class(_name, std::make_shared<concrete_creator<T, Base>>());
        };

        std::shared_ptr<Base> create(const IdType &_name) {
            auto it = factory_.find(_name);
            if (it != factory_.end()) {
                return it->second->create();
            }
            return nullptr;
        };
    protected:
        void register_class(const IdType &_name, std::shared_ptr<abstract_creator<Base>> _p) {
            auto it = factory_.find(_name);
            if (it == factory_.end()) {
                factory_.emplace(_name, _p);
            }
        }
    private:
        factory_map factory_;
    };
}
#endif //BPUTIL_FACTORY_HPP
