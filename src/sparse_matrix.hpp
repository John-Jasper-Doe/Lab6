/**
 ******************************************************************************
 * @file    sparse_matrix.hpp
 * @author  Maxim <aveter@bk.ru>
 * @date    19/05/2019
 * @brief   Description of the template "Sparse matrix".
 ******************************************************************************
 */

#ifndef SPARSE_MATRIX_HPP_
#define SPARSE_MATRIX_HPP_

#include <unistd.h>
#include <map>
#include <utility>
#include <tuple>


/**
 * @brief The sparse matrix template class.
 * @tparam value_type_t - matrix element type.
 * @tparam default_value - default value (default 0).
 * @tparam matrix_size - matrix size (default 2).
 */
template<typename value_type_t, value_type_t default_value = 0,
                                                        size_t matrix_size = 2>
class sparse_matrix
{
  private:
    /**
     * @brief The struct for generate other index type for matrix.
     * @tparam index_type_t - index type.
     * @tparam index - index.
     * @tparam types_t - other input tupes.
     */
    template<typename index_type_t, unsigned index, typename... types_t>
    struct generate_index_type {
        using type = typename generate_index_type<index_type_t,
                                                  index - 1,
                                                  index_type_t,
                                                  types_t...>::type;
    };

    /**
     * @brief The structure for generating an index type with an index of 0.
     * @tparam index_type_t - index type.
     * @tparam types_t - other input tupes.
     */
    template<typename index_type_t, typename... types_t>
    struct generate_index_type<index_type_t, 0, types_t...> {
        using type = std::tuple<types_t...>;
    };

    /**
     * @brief Class to work with matrix index.
     * @tparam num - index number.
     * @tparam types_t - index types.
     */
    template<size_t num, typename... types_t>
    class index_matrix;

    /**
     * @brief Class to work with matrix index, if index = 0.
     * @tparam types_t - index types.
     */
    template<typename... types_t>
    class index_matrix<0, types_t...>;

    /* Aliases */
    using index_t = typename generate_index_type<size_t, matrix_size>::type;
    using contener_t = typename std::map<index_t, value_type_t>;
    using next_index = index_matrix<matrix_size - 1, size_t>;


  public:
    /**
     * Class that implements an iterator for working with matrix data.
     */
    class iterator_matrix;
    class const_iterator_matrix;


    /**
     * The default constructor.
     */
    sparse_matrix() = default;

    /**
     * @brief Copy constructor.
     * @param other [in] - the object to copy.
     */
    sparse_matrix(const sparse_matrix &other)
      : data_(other.data_) {}

    /**
     * @brief Move constructor.
     * @param other [in] - the object to move.
     */
    sparse_matrix(sparse_matrix &&other)
      : data_(std::move(other.data_)) {}

    /**
     * The default destructor.
     */
    virtual ~sparse_matrix() = default;

    /**
     * @brief Copy operator.
     * @param other [in] - the object to copy.
     */
    sparse_matrix & operator=(const sparse_matrix &other) {
      if (*this != other)
        data_ = other.data_;

      return *this;
    }

    /**
     * @brief Move operator.
     * @param other [in] - the object to move.
     */
    sparse_matrix & operator=(sparse_matrix &&other) {
      data_ = std::move(other.data_);
      return *this;
    }

    /**
     * @brief Comparison operator.
     * @param other [in] - the object to move.
     */
    bool operator==(const sparse_matrix &other) const {
      return data_ == other.data_;
    }

    /**
     * @brief Access operator by index.
     * @param index [in] - matrix index.
     */
    next_index operator[](const size_t index) {
      return next_index(static_cast<std::tuple<size_t>>(index), data_);
    }

    /**
     * @brief Constant access operator by index.
     * @param index [in] - matrix index.
     */
    const next_index operator[](const size_t index) const {
      return next_index(static_cast<std::tuple<size_t>>(index), data_);
    }

    /**
     * @brief  The begin iterator of the matrix.
     * @return Returns an iterator to the beginning of the matrix.
     */
    auto begin() {
      return iterator_matrix(data_.begin());
    }

    /**
     * @brief  The const begin iterator of the matrix.
     * @return Returns an const iterator to the beginning of the matrix.
     */
    const auto cbegin() const {
      return const_iterator_matrix(data_.cbegin());
    }

    /**
     * @brief  The end iterator of the matrix.
     * @return Returns an iterator to the end of the matrix.
     */
    auto end() {
      return iterator_matrix(data_.end());
    }

    /**
     * @brief  The const end iterator of the matrix.
     * @return Returns an const iterator to the end of the matrix.
     */
    const auto cend() const {
      return const_iterator_matrix(data_.cend());
    }

    /**
     * @brief The number of calls occupied by the data matrix.
     * @return The number of occupied calls.
     */
    size_t size() const {
      return data_.size();
    }

    /**
     * Clearing the matrix data.
     */
    void clear() {
      data_.clear();
    }


  private:
    std::map<index_t, value_type_t> data_;            /**< - data container. */
    const value_type_t default_value_{default_value}; /**< - default value. */
};


/**
 * Class to work with matrix index.
 */
template<typename value_type_t, value_type_t default_value, size_t matrix_size>
template<size_t num, typename... types_t>
class sparse_matrix<value_type_t, default_value, matrix_size>::index_matrix
{
  private:
    /* Aliases */
    using next_index = index_matrix<num - 1, types_t..., size_t>;

    const sparse_matrix::contener_t &data_; /**< - container with data. */
    std::tuple<types_t...> index_view_;     /**< - representation of the index
                                             *     as a tuple. */

  public:
    /**
     * @brief The constructor.
     * @param index_view [in] - matrix index.
     * @param data [in] - contener.
     */
    index_matrix(std::tuple<types_t...> index_view,
                 const sparse_matrix::contener_t &data)
      : data_{data}, index_view_{index_view}
    {}

    /**
     * @brief Access operator by index.
     * @param index [in] - matrix index.
     */
    next_index operator[](size_t index) {
      return next_index(std::tuple_cat(index_view_, std::tie(index)), data_);
    }

    /**
     * @brief Constant access operator by index.
     * @param index [in] - matrix index.
     */
    const next_index operator[](size_t index) const {
      return next_index(std::tuple_cat(index_view_, std::tie(index)), data_);
    }
};


/**
 * Class to work with matrix index, if index = 0.
 */
template<typename value_type_t, value_type_t default_value, size_t matrix_size>
template<typename... types_t>
class sparse_matrix<value_type_t, default_value, matrix_size>::
    index_matrix<0, types_t...>
{
  private:
    const sparse_matrix::contener_t &data_;   /**< - container with data. */
    std::tuple<types_t...> index_view_;       /**< - representation of the
                                               *     index as a tuple. */
    const value_type_t default_value_{default_value};

  public:
    /**
     * @brief The constructor.
     * @param index_view [in] - matrix index.
     * @param data [in] - contener.
     */
    index_matrix(std::tuple<types_t...> index_view,
                 const sparse_matrix::contener_t &data)
      : data_{data}, index_view_{index_view}
    {}

    /**
     * @brief Copy operator.
     * @param value [in] - matrix value.
     */
    auto & operator=(const value_type_t &value) {
      if (value != default_value_)
        const_cast<sparse_matrix::contener_t &>(data_)[index_view_] = value;
      else {
        auto it = data_.find(index_view_);
        if (it != data_.cend())
          const_cast<sparse_matrix::contener_t &>(data_).erase(it);
      }
      return *this;
    }

    /**
     * Conversion operator.
     */
    operator const value_type_t &() const {
      auto it = data_.find(index_view_);
      return (it != data_.cend()) ? it->second : default_value_;
    }
};


/**
 * Class that implements an iterator for working with matrix data.
 */
template<typename value_type_t, value_type_t default_value, size_t matrix_size>
class sparse_matrix<value_type_t, default_value, matrix_size>::
                                                          const_iterator_matrix
{
  private:
    /* Aliases */
    using iterator_t = typename sparse_matrix::contener_t::const_iterator;
    using iterator_category_t = std::input_iterator_tag;

    iterator_t map_iterator_;   /**< - iterator of the data. */


  public:
   using value_t = decltype(std::tuple_cat((*map_iterator_).first,
                                          std::tie((*map_iterator_).second)));

    /**
     * @brief The constructor.
     * @param map_iterator [in] - iterator.
     */
    explicit const_iterator_matrix(iterator_t map_iterator)
      : map_iterator_(map_iterator)
    {}

    /**
     * @brief  Increment operator, prefix form.
     * @return Increment data.
     */
    const_iterator_matrix & operator++() {
      ++map_iterator_;
      return *this;
    }

    /**
     * @brief  Increment operator, postfix form.
     * @return Increment data.
     */
    const_iterator_matrix operator++(int) {
      iterator_matrix retval = *this;
      ++(*this);
      return retval;
    }

    /**
     * @brief  Comparison operator.
     * @param  other [in] - iterator.
     * @return The result of the comparison (false/true).
     */
    bool operator==(const_iterator_matrix other) const {
      return map_iterator_ == other.map_iterator_;
    }

    /**
     * @brief  Inequality operator.
     * @param  other [in] - iterator.
     * @return The result of the comparison (false/true).
     */
    bool operator!=(const_iterator_matrix other) const {
      return !(*this == other);
    }

    /**
     * @brief  Dereference operator.
     * @return Result.
     */
    value_t operator*() const {
      return std::tuple_cat((*map_iterator_).first,
                            std::tie((*map_iterator_).second));
    }
};

/**
 * Class that implements an iterator for working with matrix data.
 */
template<typename value_type_t, value_type_t default_value, size_t matrix_size>
class sparse_matrix<value_type_t, default_value, matrix_size>::iterator_matrix
{
  private:
    /* Aliases */
    using iterator_t = typename sparse_matrix::contener_t::iterator;
    using iterator_category_t = std::input_iterator_tag;

    iterator_t map_iterator_;   /**< - iterator of the data. */


  public:
   using value_t = decltype(std::tuple_cat((*map_iterator_).first,
                                          std::tie((*map_iterator_).second)));

    /**
     * @brief The constructor.
     * @param map_iterator [in] - iterator.
     */
    explicit iterator_matrix(iterator_t map_iterator)
      : map_iterator_(map_iterator)
    {}

    /**
     * @brief  Increment operator, prefix form.
     * @return Increment data.
     */
    iterator_matrix & operator++() {
      ++map_iterator_;
      return *this;
    }

    /**
     * @brief  Increment operator, postfix form.
     * @return Increment data.
     */
    iterator_matrix operator++(int) {
      iterator_matrix retval = *this;
      ++(*this);
      return retval;
    }

    /**
     * @brief  Comparison operator.
     * @param  other [in] - iterator.
     * @return The result of the comparison (false/true).
     */
    bool operator==(iterator_matrix other) const {
      return map_iterator_ == other.map_iterator_;
    }

    /**
     * @brief  Inequality operator.
     * @param  other [in] - iterator.
     * @return The result of the comparison (false/true).
     */
    bool operator!=(iterator_matrix other) const {
      return !(*this == other);
    }

    /**
     * @brief  Dereference operator.
     * @return Result.
     */
    value_t operator*() const {
      return std::tuple_cat((*map_iterator_).first,
                            std::tie((*map_iterator_).second));
    }
};

#endif /* SPARSE_MATRIX_HPP_ */
