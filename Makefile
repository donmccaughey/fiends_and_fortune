export PRODUCTS ?= products
export TMP ?= tmp


.PHONY : check
check : | test/check.out $(TMP)/check.out
	diff $|


$(TMP)/check.out : $(PRODUCTS)/fnf | $(TMP)
	$(PRODUCTS)/fnf check 0 > $@


$(TMP) :
	mkdir -p $@
