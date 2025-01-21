#pragma once

#include <klay/Layout.hpp>
#include <klay/Geometry.hpp>
#include <klay/Unit.hpp>
#include <vector>

namespace Klay {
	KLAY_DEFINE_UNIT(GridFr, float);

	/// @brief Simple grid that can be resized upwards and accessed
	/// @tparam T
	template<typename T>
	struct Grid {
		static_assert(!std::is_same_v<T, bool>, "Grid does not work with bool");

		constexpr static float growth_factor = 2.0f;
		int num_rows = 0;
		int num_cols = 0;
		int cap_rows = 0;
		int cap_cols = 0;
		std::vector<T> data;

		Grid(int rows = 0, int cols = 0, T default_value = T{})
		{
			cap_rows = std::max(static_cast<int>(growth_factor * rows), 1);
			cap_cols = std::max(static_cast<int>(growth_factor * cols), 1);
			data.resize(cap_rows * cap_cols, default_value);
			resize(rows, cols);
		}

		constexpr auto get_cell(int row, int col) const -> const T& {
			return data.at(row * cap_cols + col);
		}

		constexpr auto get_cell(int row, int col) -> T& {
			return data.at(row * cap_cols + col);
		}

		constexpr auto ensure_size(int rows, int cols, T default_value = T{}) -> bool {
			if (rows > num_rows || cols > num_cols) {
				resize(rows, cols, default_value);
				return true;
			}
			return false;
		}

		constexpr auto resize(int new_rows, int new_cols, T default_value = T{}) {
			bool capacity_changed = false;
			int new_cap_rows = cap_rows;
			int new_cap_cols = cap_cols;
			if(new_rows >= cap_rows){
				new_cap_rows = growth_factor * new_rows;
				capacity_changed = true;
			}
			if(new_cols >= cap_cols){
				new_cap_cols = growth_factor * new_cols;
				capacity_changed = true;
			}

			if(capacity_changed){
				data.resize(new_cap_rows * new_cap_cols, default_value);

				// move the existing data to their new spots
				// starting from the bottom right
				for (int row = num_rows - 1; row >= 0; --row) {
					for (int col = num_cols - 1; col >= 0; --col) {
						const auto old_index = row * cap_cols + col;
						const auto new_index = row * new_cap_cols + col;

						data[new_index] = data[old_index];
						if(old_index != new_index) {
							data[old_index] = default_value;
						}
					}
				}

				// ensure new cells are initialized
				for (int row = num_rows; row < new_rows; ++row) {
					for (int col = 0; col < new_cols; ++col) {
						data[row * new_cap_cols + col] = default_value;
					}
				}
				for (int row = 0; row < num_rows; ++row) {
					for (int col = num_cols; col < new_cols; ++col) {
						data[row * new_cap_cols + col] = default_value;
					}
				}
			}
			cap_rows = new_cap_rows;
			cap_cols = new_cap_cols;
			num_rows = new_rows;
			num_cols = new_cols;
		}
	};

	using GridExplicitTrackSize = std::variant<GridFr, Px, Percent>;

	struct GridRepeat {
		int count;
		GridExplicitTrackSize size;
	};

	struct GridTrackList {
		std::vector<GridExplicitTrackSize> sizes;

		constexpr GridTrackList() = default;

		constexpr GridTrackList(
			std::vector<GridExplicitTrackSize> sizes
		) : sizes{std::move(sizes)} {}

		constexpr GridTrackList(
			std::vector<std::variant<GridExplicitTrackSize, GridRepeat>> sizes
		) {
			for (const auto& size : sizes) {
				if (std::holds_alternative<GridExplicitTrackSize>(size)) {
					this->sizes.push_back(std::get<GridExplicitTrackSize>(size));
				} else {
					const auto& repeat = std::get<GridRepeat>(size);
					for (int i = 0; i < repeat.count; ++i) {
						this->sizes.push_back(repeat.size);
					}
				}
			}
		}
	};

	struct GridLayoutMode : LayoutMode {
		std::optional<GridTrackList> row_track_list;
		std::optional<GridTrackList> col_track_list;

		constexpr GridLayoutMode() noexcept {}

		void ComputeLayout(
			std::shared_ptr<Element> el,
			const PxRect& content_rect
		) noexcept override;

		constexpr auto GetExplicitGridSize() const noexcept -> Vector2<int> {
			return explicit_grid_size;
		}

		constexpr auto GetImplicitGridSize() const noexcept -> Vector2<int> {
			return implicit_grid_size;
		}

	private:
		Vector2<int> explicit_grid_size;
		Vector2<int> implicit_grid_size;
	};
}