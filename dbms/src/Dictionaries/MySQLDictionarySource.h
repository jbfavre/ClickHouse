#pragma once

#include <Dictionaries/IDictionarySource.h>
#include <Dictionaries/ExternalQueryBuilder.h>
#include <Dictionaries/DictionaryStructure.h>
#include <ext/range.hpp>
#include <mysqlxx/PoolWithFailover.h>
#include <Poco/Util/AbstractConfiguration.h>


namespace Poco { class Logger; }


namespace DB
{


/// Allows loading dictionaries from a MySQL database
class MySQLDictionarySource final : public IDictionarySource
{
public:
    MySQLDictionarySource(const DictionaryStructure & dict_struct_,
        const Poco::Util::AbstractConfiguration & config, const std::string & config_prefix,
        const Block & sample_block);

    /// copy-constructor is provided in order to support cloneability
    MySQLDictionarySource(const MySQLDictionarySource & other);

    BlockInputStreamPtr loadAll() override;

    BlockInputStreamPtr loadIds(const std::vector<UInt64> & ids) override;

    BlockInputStreamPtr loadKeys(
        const ConstColumnPlainPtrs & key_columns, const std::vector<std::size_t> & requested_rows) override;

    bool isModified() const override;

    bool supportsSelectiveLoad() const override;

    DictionarySourcePtr clone() const override;

    std::string toString() const override;

private:
    Poco::Logger * log;

    static std::string quoteForLike(const std::string s);

    LocalDateTime getLastModification() const;

    const DictionaryStructure dict_struct;
    const std::string db;
    const std::string table;
    const std::string where;
    const bool dont_check_update_time;
    Block sample_block;
    mutable mysqlxx::PoolWithFailover pool;
    ExternalQueryBuilder query_builder;
    const std::string load_all_query;
    LocalDateTime last_modification;
};

}
