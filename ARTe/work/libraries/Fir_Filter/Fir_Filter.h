
// Digital Control Systems and Mechatronics project - BLDC motor control
// Marco Pagani - 2015 - marco_pgn <a/t> yahoo.com

// Modified for ARTE project, correctness is not guaranteed! only for test purpose!

#ifndef FIR_FILTER_HPP_
#define FIR_FILTER_HPP_

//#include <cstdint>

// Workaround (if C++11 support is not avaiable) 
#define FIR_FILTER_TYPE(taps_array) Fir_Filter<typeof(taps_array[0] + 0), \
		sizeof(taps_array) / sizeof(taps_array[0])>

// Samples and taps array have both the
// same (num_taps) size
template <typename Data_Type, uint32_t num_taps>
class Fir_Filter {

private:
	// Taps array. Must be num_taps size
	// (passed as a parameter to the constructor)
	const Data_Type *taps_;

	// Input samples delay line
	// Implemented as a circular array (num_taps size)
	Data_Type samples_[num_taps];
	uint32_t samples_head_;

public:
	Fir_Filter(const Data_Type *taps);
	~Fir_Filter() = default;
	Fir_Filter(const Fir_Filter &) = default;
	Fir_Filter& operator=(const Fir_Filter &) = default;

	Data_Type operator()(Data_Type input);
};

template <typename Data_Type, uint32_t num_taps>
Fir_Filter<Data_Type, num_taps>::Fir_Filter(const Data_Type taps[]) :
	taps_(taps), samples_{0}, samples_head_(0)
{

}


// Filter functor. Should be O(n) in time.
// With modern optimizing compilers the array indexed access should
// be equally efficient, or even faster, than the pointer arithmetic approach.
// For sure the array indexed access makes the code more readable.
// According to ST AN4044 the STM32F4 FPU takes 1 clock cycle for Addition
// and 3 clock cycles for Multiply or multiply and accumulate (MAC).
template <typename Data_Type, uint32_t num_taps>
Data_Type Fir_Filter<Data_Type, num_taps>::operator()(const Data_Type input)
{
	// Filter output value
	Data_Type output = 0;

	// Store the new input value in the "head" position of samples array
	samples_[samples_head_] = input;

	// Traverse the samples array backwards (from the more recent to the
	// oldest) and the taps array forward (b0 to bn).
	// At each step the multiply and accumulate in the output variable

	// taps array index
	int c_idx = 0;

	// Lower part (0 <-- head_)
	for (int s_idx = samples_head_; s_idx >= 0; --s_idx)
		output += samples_[s_idx] * taps_[c_idx++];

	// Upper part (samples_head_ - 1 <-- num_taps - 1)
	for (int s_idx = num_taps - 1; s_idx > (int)samples_head_; --s_idx)
		output += samples_[s_idx] * taps_[c_idx++];

	// Update samples array head counter to the next "free" (oldest sample) position
	// Avoid using Modulo (integer remainder) operator since it generally implies
	// a computationally expensive division operation
	if (++samples_head_ > num_taps - 1)
		samples_head_ = 0;

	return output;
}

// Helper function. Class template arguments are never deduced
template <typename Data_Type, uint32_t num_taps>
Fir_Filter<Data_Type, num_taps> make_fir_filter(const Data_Type (&taps)[num_taps]) {
	return Fir_Filter<Data_Type, num_taps>(taps);
}


#endif /* FIR_FILTER_HPP_ */
