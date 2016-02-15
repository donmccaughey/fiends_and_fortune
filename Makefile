export PRODUCTS ?= products
export TMP ?= tmp


.PHONY : check
check : | tests/check.out $(TMP)/check.out
	$(PRODUCTS)/UnitTests
	diff $|


.PHONY : clean
clean :
	rm -f $(TMP)/check.out


$(TMP)/check.out : $(PRODUCTS)/fnf | $(TMP)
	$(PRODUCTS)/fnf check 0 > $@


$(TMP) :
	mkdir -p $@
